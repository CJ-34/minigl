#include "core/Shader.h"

#include <spdlog/spdlog.h>
#include <glad/glad.h>

#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>

namespace minigl {
	Shader::Shader(const std::string& v_path, const std::string& f_path) 
	: m_id(0) {
		const char* v_code = loadFile(v_path).c_str();
		spdlog::debug(v_code);
		const char* f_code = loadFile(f_path).c_str();
		spdlog::debug(f_code);

		GLuint vertex, fragment;

		vertex = compile(GL_VERTEX_SHADER, v_code);
		checkCompile(vertex, GL_VERTEX_SHADER);

		fragment = compile(GL_FRAGMENT_SHADER, f_code);
		checkCompile(fragment, GL_FRAGMENT_SHADER);

		m_id = glCreateProgram();
		glAttachShader(m_id, vertex);
		glAttachShader(m_id, fragment);
		glLinkProgram(m_id);

		checkCompile(m_id, GL_PROGRAM);

		spdlog::debug("Created shader program {} with shaders {} {}", m_id, vertex, fragment);
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_id);
	}

	void Shader::bind() const
	{
		glUseProgram(m_id);
	}

	std::string Shader::loadFile(const std::string& path) {
		std::ifstream file(path);

		if (!file.is_open()) {
			std::string msg("Failed to load file: ");
			spdlog::error(msg + path);
			return "";
		}
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
		
	}
	unsigned int Shader::compile(unsigned int type, const char* source)
	{
		GLuint shader;
		shader = glCreateShader(type);
		glShaderSource(shader, 1, &source, NULL);
		return shader;

	}
	void Shader::checkCompile(unsigned int id, unsigned int type)
	{
		int success;
		char infoLog[1024];
		
		std::string message = type == GL_VERTEX_SHADER ? "vertex: " : "fragment: ";

		if (type != GL_PROGRAM) {
			glGetShaderiv(id, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(id, sizeof(infoLog), NULL, infoLog);
				spdlog::error("failed compilation of {}: {}", message, infoLog);
			}
		}
		else {
			glGetProgramiv(id, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(id, sizeof(infoLog), NULL, infoLog);
				spdlog::error("failed to link program with id {}: {}", id, infoLog);
			}
		}
	}
}
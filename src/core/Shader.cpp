#include "core/Shader.h"

#include <spdlog/spdlog.h>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>

namespace minigl {
	Shader::Shader(const std::string& v_path, const std::string& f_path) 
	: m_id(0) {
		std::string vertex_code = loadFile(v_path);
		std::string fragment_code = loadFile(f_path);

		const char* v_code = vertex_code.c_str();
		const char* f_code = fragment_code.c_str();

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

	void Shader::setMat4(const char* name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, glm::value_ptr(mat));
	}

	void Shader::setVec3(const char* name, const glm::vec3& vec) const
	{
		glUniform3f(glGetUniformLocation(m_id, name), vec.x, vec.y, vec.x);
	}

	void Shader::setBool(const char* name, int v)
	{
		glUniform1i(glGetUniformLocation(m_id, name), v);
	}

	void Shader::setInt(const char* name, int v) const
	{
		glUniform1i(glGetUniformLocation(m_id, name), v);
	}

	void Shader::setFloat(const char* name, float v) const
	{
		glUniform1f(glGetUniformLocation(m_id, name), v);
	}

	std::string Shader::loadFile(const std::string& path) {
		std::ifstream file(path);

		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		if (!file.is_open()) {
			spdlog::error("failed to open file {}", path);
			return "";
		}
		std::stringstream buffer;
		buffer << file.rdbuf();
		
		file.close();

		return buffer.str();
		
	}
	unsigned int Shader::compile(unsigned int type, const char* source)
	{
		GLuint shader;
		shader = glCreateShader(type);
		glShaderSource(shader, 1, &source, NULL);
		glCompileShader(shader);
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
				spdlog::error("failed compilation of {} shader: {}", message, infoLog);
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
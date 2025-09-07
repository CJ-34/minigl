#pragma once

#include <glm/glm.hpp>
#include <string>

#define GL_PROGRAM 0x8B29 // own definition

namespace minigl {
	class Shader {
	public:
		Shader(const std::string& v_path, const std::string& f_path);
		~Shader();
		void bind() const;
		void setMat4(const char* name, const glm::mat4& mat) const;
		void setVec3(const char* name, const glm::vec3& vec) const;
		void setBool(const char* name, int v);
		void setInt(const char* name, int v) const;
		void setFloat(const char* name, float v) const;
	private:
		unsigned int m_id;
		static std::string loadFile(const std::string& path);
		static unsigned int compile(unsigned int type, const char* source);
		static void checkCompile(unsigned int id, unsigned int type);
	};
}
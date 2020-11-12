#pragma once
#include <string>
#include <unordered_map>
#include "math/Lingal.h"
#include "GLOjbect.h"
#include "glm/glm.hpp"

namespace OpenGL
{
	using namespace Math;
	struct ShaderProgramSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	class Shader : public GLObject
	{
	private:
		std::string mFilePath;
		std::unordered_map<std::string, int> mLocationCache;
	public:
		Shader();
		Shader(const std::string& filepath);
		~Shader();

		void AttachProgram(const std::string& filepath);
		void Bind() const;
		void UnBind() const;
		void SetUniform1i(const std::string& name, int v0);
		void SetUniform1f(const std::string& name, float v0);
		void SetUniform2f(const std::string& name, float v0, float v1);
		void SetUniform3f(const std::string& name, float v0, float v1, float v2);
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void SetUniformMat4f(const std::string& name, const glm::mat4 &mat);
		void SetUniformMat4f(const std::string& name, const Matrix4& mat);
	private:
		unsigned int GetUniformLocation(const std::string& name);
		ShaderProgramSource ParseShader(const std::string& filepath);
		unsigned int CompileShader(unsigned int type, const std::string& source);
		int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	};
}
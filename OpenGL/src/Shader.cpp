#include "Shader.h"
#include "Renderer.h"
#include "math/Lingal.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


namespace OpenGL
{
	void ConvertToGLMatrix(const Matrix4& mat, float* buf)
	{
		int idx = 0;
		for (int i = 0; i < 4; i++) 
		{
			const Vector4& col = mat[i];
			buf[idx++] = col[0]; buf[idx++] = col[1]; buf[idx++] = col[2]; buf[idx++] = col[3];
		}
	}

	Shader::Shader(const std::string& filepath) : mFilePath(filepath), GLObject(-1)
	{
		ShaderProgramSource source = ParseShader(filepath);
		mHandle = CreateShader(source.VertexSource, source.FragmentSource);
	}

	Shader::Shader(): GLObject(-1)
	{

	}

	Shader::~Shader()
	{
		GLCall(glDeleteProgram(mHandle));
	}

	void Shader::AttachProgram(const std::string& filepath)
	{
		mFilePath = filepath;
		ShaderProgramSource source = ParseShader(filepath);
		mHandle = CreateShader(source.VertexSource, source.FragmentSource);
	}

	void Shader::Bind() const
	{
		GLCall(glUseProgram(mHandle));
	}

	void Shader::UnBind() const
	{
		GLCall(glUseProgram(0));
	}

	void Shader::SetUniform1i(const std::string& name, int v0)
	{
		GLCall(glUniform1i(GetUniformLocation(name), v0));
	}

	void Shader::SetUniform1f(const std::string& name, float v0)
	{
		GLCall(glUniform1f(GetUniformLocation(name), v0));
	}

	void Shader::SetUniform2f(const std::string& name, float v0, float v1)
	{
		GLCall(glUniform2f(GetUniformLocation(name), v0, v1));
	}

	void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
	{
		GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
	}

	void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
	{

		GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
	}

	void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& mat)
	{
		GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
	}

	void Shader::SetUniformMat4f(const std::string& name, const Matrix4& mat)
	{
		float buf[16];
		ConvertToGLMatrix(mat, buf);
		GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, buf));
	}

	unsigned int Shader::GetUniformLocation(const std::string& name)
	{
		if (mLocationCache.find(name) != mLocationCache.end())
			return mLocationCache[name];
		GLCall(int location = glGetUniformLocation(mHandle, name.c_str()));
		if (location == -1)
			std::cout << "Warming: uniform " << name << " doesn't exist!" << std::endl;
		mLocationCache[name] = location;
		return location;
	}



	ShaderProgramSource Shader::ParseShader(const std::string& filepath)
	{
		std::fstream stream(filepath);
		std::string line;
		std::stringstream ss[2];
		enum class ShaderType
		{
			NONE = -1, VERTEX = 0, FRAGMENT = 1
		};
		ShaderType type = ShaderType::NONE;
		while (std::getline(stream, line))
		{
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
					type = ShaderType::VERTEX;
				else if (line.find("fragment") != std::string::npos)
					type = ShaderType::FRAGMENT;
			}
			else
			{
				ss[(int)type] << line << "\n";
			}
		}
		return { ss[0].str(), ss[1].str() };
	}

	unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
	{
		unsigned int id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);
			std::cout << "Failed to compile shader" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
			std::cout << message << std::endl;
			glDeleteShader(id);
			return 0;
		}
		return id;
	}

	int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
	{
		unsigned int program = glCreateProgram();
		unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
		unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		glValidateProgram(program);

		glDeleteShader(vs);
		glDeleteShader(fs);

		return program;
	}
}
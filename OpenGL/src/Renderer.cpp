#include "Renderer.h"
#include <iostream>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

namespace OpenGL
{
	void GLClearError()
	{
		//std::cout << std::hex << glGetError() << std::endl;
		int err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			std::cout << std::hex << err << std::endl;
		};
	}

	bool GLLogCall(const char* function, const char* file, int line)
	{
		while (GLenum error = glGetError())
		{
			std::cout << "[OpenGL Error] (" << error << ") " <<
				function << " " << file << ": " << line << std::endl;
			return false;
		}
		return true;
	}

	Renderer::Renderer()
	{

	}

	void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
	{
		shader.Bind();
		va.Bind();
		ib.Bind();
		GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
	}

	void Renderer::Clear() const
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void Renderer::EnableDepthTest() const
	{
		GLCall(glEnable(GL_DEPTH_TEST));
	}

	void Renderer::DisableDepthTest() const
	{
		GLCall(glDisable(GL_DEPTH_TEST));
	}

}
#include "OpenGL.h"
#include <iostream>
namespace OpenGL
{
	void GLClearError()
	{
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
}


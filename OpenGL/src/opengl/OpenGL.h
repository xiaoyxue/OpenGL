#pragma once
#include <GL/glew.h>

namespace OpenGL
{
#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

#define GLCallNoError(x) \
    do \
    {\
        x; \
        glGetError();\
    } while (0)

	void GLClearError();
	bool GLLogCall(const char* function, const char* file, int line);
}

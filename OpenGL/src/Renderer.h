#pragma once

#include <GL/glew.h>

namespace OpenGL
{

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))


    void GLClearError();
    bool GLLogCall(const char* function, const char* file, int line);


    class VertexArray;
    class IndexBuffer;
    class Shader;

    class Renderer
    {
    private:
    public:
        Renderer();
        void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
        void Clear() const;
        void EnableDepthTest() const;
        void DisableDepthTest() const;
    };

}
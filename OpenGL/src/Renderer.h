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
    class Camera;
    class DrawableObject;

    class Renderer
    {
    private:
        Camera* mpCamera;
        int mWidth, mHeight;
		bool mMouseLeftDown;
		bool mMouseRightDown;
		bool mMouseMiddleDown;
        bool mDrawWireFrame;
		float mMouseX, mMouseY;
    public:
        Renderer();
        void Init();
        void Resize(int width, int height);
        void SetSize(int width, int height);
        void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
        void DrawFace(const DrawableObject &object);
        void DrawWireFrame(const DrawableObject& object);
        void SetCamera(Camera *camera);
        void Clear() const;
        void EnableDepthTest() const;
        void DisableDepthTest() const;

        inline Camera* GetCamera() const { return mpCamera; }

		void MouseButtonEvent(int button, int action, int mods);
		void KeyBoardEvent(int key, int event, int mods, float deltaTime);
		void CursorEvent(float x, float y);

        static unsigned int IsReady();
    private:

        void MouseRightDrag(float x, float y);
        void MouseLeftDrag(float x, float y);
        void MouseMiddleDrag(float x, float y);
    };

}
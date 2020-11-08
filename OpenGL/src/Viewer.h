#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>


namespace OpenGL 
{
	class Renderer;
	class Camera;
	class DrawableObject;
	class Shader;

	class Viewer
	{
	private:
		GLFWwindow* mWindow;
		static Renderer* mRenderer;
		static Camera* mCamera;
		static Shader* mShader;
		static int mWidth, mHeight;
		static float mDeltaTime, mLastFrame;
		std::string mTitle;
		std::vector<DrawableObject*> mMeshes;

		static bool mMouseLeftDown;
		static bool mMouseRightDown;
		static bool mMouseMiddleDown;
		static float mMouseX, mMouseY;
	public:
		Viewer(const std::string &title);
		~Viewer();
		void Init();
		void Start();
		void SetRenderer(Renderer* renderer);
		void SetCamera(Camera* camera);
		void SetSize(int width, int height);
		void SetShader(Shader* shader);
		//TO REMOVE
		void Add(DrawableObject* obj) { mMeshes.push_back(obj); }
		void Draw() const;

		static void ErrorCallback(int error, const char* description);
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void ResizeCallback(GLFWwindow* window, int width, int height);
		static void CursorCallback(GLFWwindow* window, double xpos, double ypos);
		static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

		static void MouseDrag(float x, float y);
	};
}
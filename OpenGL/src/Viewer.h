#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>


namespace OpenGL 
{
	class Renderer;
	class DrawableObject;
	class Shader;

	class Viewer
	{
	private:
		GLFWwindow* mpWindow;
		std::string mTitle;
		std::vector<DrawableObject*> mMeshes;
		static Renderer* mpRenderer;
		static int mWidth, mHeight;
		static float mDeltaTime, mLastFrame;

	public:
		Viewer(const std::string &title);
		~Viewer();
		void Init();
		void Start();
		void SetRenderer(Renderer* renderer);
		void SetSize(int width, int height);
		//TO REMOVE
		void Add(DrawableObject* obj) { mMeshes.push_back(obj); }
		void Draw() const;

		static void ErrorCallback(int error, const char* description);
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void ResizeCallback(GLFWwindow* window, int width, int height);
		static void CursorCallback(GLFWwindow* window, double xpos, double ypos);
		static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	};
}
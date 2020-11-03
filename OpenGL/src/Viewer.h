#pragma once
#include <GLFW/glfw3.h>
#include <string>

namespace OpenGL 
{
	class Renderer;
	class Camera;

	class Viewer
	{
	private:
		Renderer* mRenderer;
		GLFWwindow* mWindow;
		Camera* mCamera;
		int mWidth, mHeight;
		std::string mTitle;

	public:
		Viewer(const std::string &title);
		~Viewer();
		void Init();
		void Start();
		void SetRenderer(Renderer* renderer);
		void SetCamera(Camera* camera);
		void SetSize(int width, int height);
		void Draw() const;

		static void ErrorCallback(int error, const char* description);
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void ResizeCallback(GLFWwindow* window, int width, int height);
		static void CursorCallback(GLFWwindow* window, double xpos, double ypos);
		static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	};
}
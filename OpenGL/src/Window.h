#pragma once
#include "OpenGL.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "ForwardDecl.h"

namespace GLFW
{
	using namespace OpenGL;

	class Window
	{
	protected:
		static int mWidth, mHeight;
		std::string mTitle;
	public:
		Window() = default;
		Window(const std::string& title, int w = 1024, int h = 768)
			:mTitle(title)
		{
			mWidth = w;
			mHeight = h;
		}
		virtual void SetSize(int w, int h);
		virtual void MainLoop() {}
		virtual ~Window() = default;
	};

	class GLFWWindow : public Window
	{
	
	private:
		GLFWwindow* mpWindow;
		static float mDeltaTime, mLastFrame;
	protected:
		static Renderer* mpRenderer;
	public:
		GLFWWindow() = default;
		GLFWWindow(const std::string& title, int w = 1024, int h = 760);
		virtual ~GLFWWindow();

		void Init();
		void MainLoop();
		void SetRenderer(OpenGL::Renderer* pRenderer);
		virtual void DrawAll() const {};

	private:

		static void ErrorCallback(int error, const char* description);
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void ResizeCallback(GLFWwindow* window, int width, int height);
		static void CursorCallback(GLFWwindow* window, double xpos, double ypos);
		static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	};

}

#pragma once
#include "Window.h"
#include "ForwardDecl.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>

namespace GLFW
{
	using namespace OpenGL;

	class GLWindow : public Window
	{
	protected:
		GLFWwindow* mpWindow;
		Renderer* mpRenderer;
		double mDeltaTime, mLastRefreshTime, mLastFrameTime;
		double mMaxFps, mInvFpsLimit;
		double mFps;
		long long mFrameCount = 0;
	public:
		GLWindow() = default;
		GLWindow(const std::string& title, int w = 1024, int h = 760);
		virtual ~GLWindow();

		void Init();
		void MainLoop();
		void SetRenderer(Renderer* pRenderer);
		void SetMaxFps(double fps = 60);
		inline GLFWwindow* GetGlfwWindow() const { return mpWindow; }
		virtual void DrawAll() const;

	private:
		void SetErrorCallback();
		void SetKeyCallback();
		void SetResizeCallback();
		void SetCursorCallback();
		void SetScrollCallback();
		void SetMouseButtonCallback();


		static void ErrorCallback(int error, const char* description);
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void ResizeCallback(GLFWwindow* window, int width, int height);
		static void CursorCallback(GLFWwindow* window, double xpos, double ypos);
		static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	protected:
		virtual void MouseButtonCallbackFunc(int button, int action, int mods) {}
		virtual void ScrollCallbackFunc(double xoffset, double yoffset) {}
		virtual void CursorCallbackFunc(double xpos, double ypos) {}
		virtual void ResizeCallbackFunc(int width, int height) {}
		virtual void KeyCallbackFunc(int key, int scancode, int action, int mods) {}
	};
}
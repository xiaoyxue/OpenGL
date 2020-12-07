#include "GLWindow.h"
#include "opengl/Renderer.h"
#include "opengl/Misc.h"
#include <iostream>
#include <map>


namespace GLFW
{
	std::map<GLFWwindow*, GLWindow*> __window_to_instances__;

	GLWindow::GLWindow(const std::string& title, int w /*= 1024*/, int h /*= 760*/)
		:Window(title, w, h), mpWindow(nullptr), mpRenderer(nullptr), mLastRefreshTime(0), mDeltaTime(0)
	{

	}

	GLWindow::~GLWindow()
	{
		glfwDestroyWindow(mpWindow);
		glfwTerminate();
	}

	void GLWindow::Init()
	{
		if (!glfwInit())
		{
			std::cout << "Error: GLFW can not create window!" << std::endl;
			exit(-1);
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

		mpWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), NULL, NULL);
		if (!mpWindow)
		{
			glfwTerminate();
			exit(-1);
		}
		__window_to_instances__[mpWindow] = this;
		glfwMakeContextCurrent(mpWindow);
		glfwSwapInterval(1);

		//Register callback functions
		// framebuffer event callbacks
		SetResizeCallback();

		// key event callbacks
		SetKeyCallback();

		// cursor event callbacks
		SetCursorCallback();

		// wheel event callbacks
		SetScrollCallback();

		SetMaxFps();

		// mouse button callbacks
		glfwSetInputMode(mpWindow, GLFW_STICKY_MOUSE_BUTTONS, 1);
		SetMouseButtonCallback();

		// initialize renderer
		if (Renderer::IsReady() != RENDERER_OK)
		{
			std::cout << "Error: could not initialize renderer!" << std::endl;
			glfwTerminate();
			exit(-1);
		}

		if (mpRenderer)
			mpRenderer->Init();
	}

	void GLWindow::MainLoop()
	{
		mpRenderer->EnableDepthTest();
		while (!glfwWindowShouldClose(mpWindow))
		{
			double currentTime = glfwGetTime();
			mDeltaTime = currentTime - mLastRefreshTime;
			// Limit the max fps
			if ((currentTime - mLastFrameTime) >= mInvFpsLimit)
			{
				DrawAll();
				glfwSwapBuffers(mpWindow);
				mLastFrameTime = currentTime;
			}
			mLastRefreshTime = currentTime;
			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	void GLWindow::SetRenderer(Renderer* pRenderer)
	{
		mpRenderer = pRenderer;
	}

	void GLWindow::SetMaxFps(double fps /*= 60*/)
	{
		mMaxFps = fps;
		mInvFpsLimit = 1.0 / mMaxFps;
	}

	void GLWindow::DrawAll() const
	{

	}

	void GLWindow::SetErrorCallback()
	{
		glfwSetErrorCallback(ErrorCallback);
	}

	void GLWindow::SetKeyCallback()
	{
		glfwSetKeyCallback(mpWindow, KeyCallback);

	}

	void GLWindow::SetResizeCallback()
	{
		glfwSetFramebufferSizeCallback(mpWindow, ResizeCallback);

	}

	void GLWindow::SetCursorCallback()
	{
		glfwSetCursorPosCallback(mpWindow, CursorCallback);

	}

	void GLWindow::SetScrollCallback()
	{
		glfwSetScrollCallback(mpWindow, ScrollCallback);

	}

	void GLWindow::SetMouseButtonCallback()
	{
		glfwSetMouseButtonCallback(mpWindow, MouseButtonCallback);

	}

	void GLWindow::ErrorCallback(int error, const char* description)
	{
		std::cout << "GLFW error: " << error << ", " << description << std::endl;
	}

	void GLWindow::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (__window_to_instances__.find(window) == __window_to_instances__.end())
			return;
		auto it = __window_to_instances__[window];
		if (action == GLFW_PRESS) {
			if (key == GLFW_KEY_ESCAPE) {
				glfwSetWindowShouldClose(window, true);
			}
		}
		it->KeyCallbackFunc(key, scancode, action, mods);
	}

	void GLWindow::ResizeCallback(GLFWwindow* window, int width, int height)
	{
		if (__window_to_instances__.find(window) == __window_to_instances__.end())
			return;
		auto it = __window_to_instances__[window];
		int w, h;
		glfwGetFramebufferSize(window, &w, &h);
		it->mWidth = w;
		it->mHeight = h;
		it->ResizeCallbackFunc(it->mWidth, it->mHeight);
	}

	void GLWindow::CursorCallback(GLFWwindow* window, double xpos, double ypos)
	{
		if (__window_to_instances__.find(window) == __window_to_instances__.end())
			return;
		auto it = __window_to_instances__[window];
		it->CursorCallbackFunc(xpos, ypos);
	}

	void GLWindow::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		if (__window_to_instances__.find(window) == __window_to_instances__.end())
			return;
		auto it = __window_to_instances__[window];
		it->ScrollCallbackFunc(xoffset, yoffset);
	}

	void GLWindow::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		if (__window_to_instances__.find(window) == __window_to_instances__.end())
			return;
		auto it = __window_to_instances__[window];
		it->MouseButtonCallbackFunc(button, action, mods);
	}

}


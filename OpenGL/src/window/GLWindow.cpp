#include "GLWindow.h"
#include "../Renderer.h"
#include "../Misc.h"
#include "imgui/imgui.h"
#include <iostream>
#include <unordered_map>
namespace GLFW
{
	std::unordered_map<GLFWwindow*, GLWindow*> __GLWindows;

	GLWindow::GLWindow(const std::string& title, int w /*= 1024*/, int h /*= 760*/)
		:Window(title, w, h), mpWindow(nullptr), mpRenderer(nullptr), mLastFrame(0), mDeltaTime(0)
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
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		mpWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), NULL, NULL);
		if (!mpWindow)
		{
			glfwTerminate();
			exit(-1);
		}
		__GLWindows[mpWindow] = this;
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
			float currentFrame = float(glfwGetTime());
			mDeltaTime = currentFrame - mLastFrame;
			mLastFrame = currentFrame;
			DrawAll();


			glfwSwapBuffers(mpWindow);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	void GLWindow::SetRenderer(Renderer* pRenderer)
	{
		mpRenderer = pRenderer;
	}

	void GLWindow::DrawAll() const
	{

	}

	void GLWindow::SetErrorCallback()
	{
		glfwSetErrorCallback(
			[](int error, const char* description)
			{
				std::cout << "GLFW error: " << error << ", " << description << std::endl;
			});
	}

	void GLWindow::SetKeyCallback()
	{
		glfwSetKeyCallback(mpWindow, 
			[](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				if (__GLWindows.find(window) == __GLWindows.end())
					return;
				auto it = __GLWindows[window];
				if (action == GLFW_PRESS) {
					if (key == GLFW_KEY_ESCAPE) {
						glfwSetWindowShouldClose(window, true);
					}
				}
				it->mpRenderer->KeyBoardEvent(key, scancode, action, it->mDeltaTime);
			});
	}

	void GLWindow::SetResizeCallback()
	{
		glfwSetFramebufferSizeCallback(mpWindow, 
			[](GLFWwindow* window, int width, int height)
			{
				if (__GLWindows.find(window) == __GLWindows.end())
					return;
				auto it = __GLWindows[window];
				int w, h;
				glfwGetFramebufferSize(window, &w, &h);
				it->mWidth = w;
				it->mHeight = h;
				it->mpRenderer->Resize(it->mWidth, it->mHeight);
			});
	}

	void GLWindow::SetCursorCallback()
	{
		glfwSetCursorPosCallback(mpWindow, 
			[](GLFWwindow* window, double xpos, double ypos)
			{
				if (__GLWindows.find(window) == __GLWindows.end())
					return;
				auto it = __GLWindows[window];
				it->mpRenderer->CursorEvent((float)xpos, (float)ypos);
			});
	}

	void GLWindow::SetScrollCallback()
	{
		glfwSetScrollCallback(mpWindow, 
			[](GLFWwindow* window, double xoffset, double yoffset)
			{

			});
	}

	void GLWindow::SetMouseButtonCallback()
	{
		glfwSetMouseButtonCallback(mpWindow, 
			[](GLFWwindow* window, int button, int action, int mods)
			{
				if (__GLWindows.find(window) == __GLWindows.end())
					return;
				auto it = __GLWindows[window];

				if (!it->HandleGLMouseEvent())
					return;
				it->mpRenderer->MouseButtonEvent(button, action, mods);
			});
	}


}


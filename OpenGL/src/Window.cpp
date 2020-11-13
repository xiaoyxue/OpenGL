#include "Window.h"
#include <iostream>
#include "Misc.h"
#include "Renderer.h"

namespace GLFW
{
	int Window::mWidth;

	int Window::mHeight;

	void Window::SetSize(int w, int h)
	{
		mWidth = w;
		mHeight = h;
	}

	float GLFWWindow::mDeltaTime;
	float GLFWWindow::mLastFrame;

	Renderer* GLFWWindow::mpRenderer;

	GLFWWindow::GLFWWindow(const std::string& title, int w /*= 1024*/, int h /*= 760*/)
		:Window(title, w, h), mpWindow(nullptr)
	{

	}

	GLFWWindow::~GLFWWindow()
	{
		glfwDestroyWindow(mpWindow);
		glfwTerminate();
	}

	void GLFWWindow::Init()
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
		glfwMakeContextCurrent(mpWindow);
		glfwSwapInterval(1);

		//Register callback functions
		// framebuffer event callbacks
		glfwSetFramebufferSizeCallback(mpWindow, ResizeCallback);

		// key event callbacks
		glfwSetKeyCallback(mpWindow, KeyCallback);

		// cursor event callbacks
		glfwSetCursorPosCallback(mpWindow, CursorCallback);

		// wheel event callbacks
		glfwSetScrollCallback(mpWindow, ScrollCallback);

		// mouse button callbacks
		glfwSetInputMode(mpWindow, GLFW_STICKY_MOUSE_BUTTONS, 1);
		glfwSetMouseButtonCallback(mpWindow, MouseButtonCallback);

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

	void GLFWWindow::MainLoop()
	{
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

	void GLFWWindow::SetRenderer(OpenGL::Renderer* pRenderer)
	{
		mpRenderer = pRenderer;
	}

	void GLFWWindow::ErrorCallback(int error, const char* description)
	{

	}

	void GLFWWindow::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (action == GLFW_PRESS) {
			if (key == GLFW_KEY_ESCAPE) {
				glfwSetWindowShouldClose(window, true);
			}
		}
		mpRenderer->KeyBoardEvent(key, scancode, action, mDeltaTime);
	}

	void GLFWWindow::ResizeCallback(GLFWwindow* window, int width, int height)
	{
		int w, h;
		glfwGetFramebufferSize(window, &w, &h);
		mWidth = w;
		mHeight = h;
		mpRenderer->Resize(mWidth, mHeight);
	}

	void GLFWWindow::CursorCallback(GLFWwindow* window, double xpos, double ypos)
	{
		mpRenderer->CursorEvent((float)xpos, (float)ypos);
	}

	void GLFWWindow::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		mpRenderer->MouseButtonEvent(button, action, mods);
	}

	void GLFWWindow::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{

	}

}


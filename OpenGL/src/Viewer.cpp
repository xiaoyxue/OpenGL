#include "Viewer.h"
#include "Camera.h"
#include <iostream>

namespace OpenGL
{

	Viewer::Viewer(const std::string& title)
		: mTitle(title)
	{

	}

	Viewer::~Viewer()
	{
		glfwDestroyWindow(mWindow);
		glfwTerminate();
	}

	void Viewer::Init()
	{
		if (!glfwInit())
		{
			std::cout << "Error: GLFW can not create window!" << std::endl;
			exit(-1);
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		mWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), NULL, NULL);
		if (!mWindow)
		{
			glfwTerminate();
			exit(-1);
		}
		glfwMakeContextCurrent(mWindow);
		glfwSwapInterval(1);

		//Register callback functions
		// framebuffer event callbacks
		glfwSetFramebufferSizeCallback(mWindow, ResizeCallback);

		// key event callbacks
		glfwSetKeyCallback(mWindow, KeyCallback);

		// cursor event callbacks
		glfwSetCursorPosCallback(mWindow, CursorCallback);

		// wheel event callbacks
		glfwSetScrollCallback(mWindow, ScrollCallback);

		// mouse button callbacks
		glfwSetInputMode(mWindow, GLFW_STICKY_MOUSE_BUTTONS, 1);
		glfwSetMouseButtonCallback(mWindow, MouseButtonCallback);
	}

	void Viewer::Start()
	{

	}


	void Viewer::SetRenderer(Renderer* renderer)
	{
		mRenderer = renderer;
	}

	void Viewer::SetCamera(Camera* camera)
	{
		mCamera = camera;
	}

	void Viewer::SetSize(int width, int height)
	{
		mWidth = width;
		mHeight = height;
		mCamera->Resize(mWidth, mHeight);
	}

	void Viewer::ErrorCallback(int error, const char* description)
	{

	}

	void Viewer::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{

	}

	void Viewer::ResizeCallback(GLFWwindow* window, int width, int height)
	{

	}

	void Viewer::CursorCallback(GLFWwindow* window, double xpos, double ypos)
	{

	}

	void Viewer::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{

	}

	void Viewer::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{

	}


}
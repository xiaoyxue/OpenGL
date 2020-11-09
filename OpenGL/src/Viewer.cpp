#include "Viewer.h"
#include "Camera.h"
#include "Shader.h"
#include "Renderer.h"
#include "Misc.h"
#include <iostream>
namespace OpenGL
{

	OpenGL::Renderer* Viewer::mpRenderer = nullptr;

	OpenGL::Shader* Viewer::mpShader = nullptr;
	
	int Viewer::mWidth, Viewer::mHeight;

	float Viewer::mDeltaTime, Viewer::mLastFrame;

	Viewer::Viewer(const std::string& title)
		: mTitle(title)
	{

	}

	Viewer::~Viewer()
	{
		glfwDestroyWindow(mpWindow);
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

		// initialize glew
		if (glewInit() != GLEW_OK) 
		{
			std::cout << "Error: could not initialize GLEW!" << std::endl;
			glfwTerminate();
			exit(-1);
		}

		if (mpRenderer)
		{
			mpRenderer->Init();
		}
	}

	void Viewer::Start()
	{
		while (!glfwWindowShouldClose(mpWindow))
		{
			float currentFrame = float(glfwGetTime());
			mDeltaTime = currentFrame - mLastFrame;
			mLastFrame = currentFrame;
			Draw();
		}
	}


	void Viewer::SetRenderer(Renderer* renderer)
	{
		mpRenderer = renderer;
	}


	void Viewer::SetSize(int width, int height)
	{
		mWidth = width;
		mHeight = height;
		mpRenderer->Resize(width, height);
	}

	void Viewer::SetShader(Shader* shader)
	{
		mpShader = shader;
	}

	void Viewer::Draw() const
	{
		mpRenderer->Clear();
		for (auto it : mMeshes)
		{
			mpShader->Bind();
			//mCamera->Rotate(-0.05f, 0.f);
			glm::mat4 proj = mpRenderer->GetCamera()->GetProjMatrix();
			glm::mat4 view = mpRenderer->GetCamera()->GetViewMatrix();
			glm::mat4 model = glm::mat4(1.f);
			//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
			mpShader->SetUniformMat4f("u_Model", model);
			mpShader->SetUniformMat4f("u_View", view);
			mpShader->SetUniformMat4f("u_Proj", proj);
			mpShader->UnBind();
			it->Draw(*mpRenderer, *mpShader);
		}

		/* Swap front and back buffers */
		glfwSwapBuffers(mpWindow);

		/* Poll for and process events */
		glfwPollEvents();
	}

	void Viewer::ErrorCallback(int error, const char* description)
	{

	}

	void Viewer::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (action == GLFW_PRESS) {
			if (key == GLFW_KEY_ESCAPE) {
				glfwSetWindowShouldClose(window, true);
			}
		}
		mpRenderer->KeyBoardEvent(key, scancode, action, mDeltaTime);
	}

	void Viewer::ResizeCallback(GLFWwindow* window, int width, int height)
	{

	}

	void Viewer::CursorCallback(GLFWwindow* window, double xpos, double ypos)
	{
		mpRenderer->CursorEvent((float)xpos, (float)ypos);
	}

	void Viewer::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{

	}

	void Viewer::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		mpRenderer->MouseButtonEvent(button, action, mods);
	}


}
#include "Viewer.h"
#include "Camera.h"
#include "Shader.h"
#include "Renderer.h"
#include <iostream>
namespace OpenGL
{

	OpenGL::Renderer* Viewer::mRenderer = nullptr;

	OpenGL::Camera* Viewer::mCamera = nullptr;

	OpenGL::Shader* Viewer::mShader = nullptr;

	int Viewer::mWidth, Viewer::mHeight;

	float Viewer::mDeltaTime, Viewer::mLastFrame;

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

		// initialize glew
		if (glewInit() != GLEW_OK) 
		{
			std::cout << "Error: could not initialize GLEW!" << std::endl;
			glfwTerminate();
			exit(-1);
		}
	}

	void Viewer::Start()
	{
		while (!glfwWindowShouldClose(mWindow))
		{
			float currentFrame = glfwGetTime();
			mDeltaTime = currentFrame - mLastFrame;
			mLastFrame = currentFrame;
			Draw();
		}
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

	void Viewer::SetShader(Shader* shader)
	{
		mShader = shader;
	}

	void Viewer::Draw() const
	{
		mRenderer->Clear();
		for (auto it : mMeshes)
		{
			mShader->Bind();
			mCamera->Rotate(-0.05f, 0.f);
			glm::mat4 proj = mCamera->GetProjMatrix();
			glm::mat4 view = mCamera->GetViewMatrix();
			glm::mat4 model = glm::mat4(1.f);
			//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
			mShader->SetUniformMat4f("u_Model", model);
			mShader->SetUniformMat4f("u_View", view);
			mShader->SetUniformMat4f("u_Proj", proj);
			mShader->UnBind();
			it->Draw(*mRenderer, *mShader);
		}

		/* Swap front and back buffers */
		glfwSwapBuffers(mWindow);

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
		if (key == GLFW_KEY_W) {
			mCamera->MoveY(mDeltaTime);
		}
		else if (key == GLFW_KEY_S) {
			mCamera->MoveY(-mDeltaTime);
		}
		else if (key == GLFW_KEY_A) {
			mCamera->MoveX(-mDeltaTime);
		}
		else if (key == GLFW_KEY_D) {
			mCamera->MoveX(mDeltaTime);
		}
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
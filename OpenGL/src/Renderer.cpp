#include "Renderer.h"
#include <iostream>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Misc.h"
#include "Camera.h"
#include "GLOjbect.h"

namespace OpenGL
{
	void GLClearError()
	{
		int err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			std::cout << std::hex << err << std::endl;
		};
	}

	bool GLLogCall(const char* function, const char* file, int line)
	{
		while (GLenum error = glGetError())
		{
			std::cout << "[OpenGL Error] (" << error << ") " <<
				function << " " << file << ": " << line << std::endl;
			return false;
		}
		return true;
	}

	Renderer::Renderer()
	{
		mWidth = DEFAULT_RESOLUTION_X;
		mHeight = DEFAULT_RESOLUTION_Y;
		mMouseLeftDown = false;
		mMouseRightDown = false;
		mMouseMiddleDown = false;
		mMouseX = false;
		mMouseY = false;
		mpCamera = nullptr;
		mDrawWireFrame = true;
	}

	void Renderer::Init()
	{
		GLCall(glEnable(GL_LINE_SMOOTH));
		GLCall(glEnable(GL_POLYGON_SMOOTH));
		GLCall(glHint(GL_LINE_SMOOTH_HINT, GL_NICEST));
		GLCall(glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST));
	}

	void Renderer::Resize(int width, int height)
	{
		mWidth = width;
		mHeight = height;
		GLCall(glViewport(0, 0, mWidth, mHeight));
		mpCamera->Resize(mWidth, mHeight);
	}

	void Renderer::SetSize(int width, int height)
	{
		mWidth = width;
		mHeight = height;
	}

	void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
	{
		shader.Bind();
		va.Bind();
		ib.Bind();

		GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
	}

	void Renderer::DrawFace(const DrawableObject& object)
	{
		GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
		GLCall(glEnable(GL_POLYGON_OFFSET_FILL));
		GLCall(glPolygonOffset(1.0, 1.0));
		object.DrawFace(*this);
	}

	void Renderer::DrawWireFrame(const DrawableObject& object)
	{
		GLCall(glDisable(GL_POLYGON_OFFSET_FILL));
		GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		object.DrawWireFrame(*this);
	}

	void Renderer::SetCamera(Camera* camera)
	{
		mpCamera = camera;
	}

	void Renderer::Clear() const
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void Renderer::EnableDepthTest() const
	{
		GLCall(glEnable(GL_DEPTH_TEST));
	}

	void Renderer::DisableDepthTest() const
	{
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void Renderer::MouseButtonEvent(int button, int action, int mods)
	{
		if (button == MOUSE_LEFT)
		{
			if (action == MOUSE_BUTTON_PRESS)
				mMouseLeftDown = true;
			else
				mMouseLeftDown = false;
		}
		else if (button == MOUSE_RIGHT)
		{
			if (action == MOUSE_BUTTON_PRESS)
				mMouseRightDown = true;
			else
				mMouseRightDown = false;
		}
		else if (button == MOUSE_MIDDLE)
		{
			if (action == MOUSE_BUTTON_PRESS)
				mMouseMiddleDown = true;
			else
				mMouseMiddleDown = false;
		}
	}

	void Renderer::KeyBoardEvent(int key, int event, int mods, float deltaTime)
	{
		if (key == KEYBOARD_W) {
			mpCamera->MoveY(deltaTime);
		}
		else if (key == KEYBOARD_S) {
			mpCamera->MoveY(-deltaTime);
		}
		else if (key == KEYBOARD_A) {
			mpCamera->MoveX(-deltaTime);
		}
		else if (key == KEYBOARD_D) {
			mpCamera->MoveX(deltaTime);
		}
	}

	void Renderer::CursorEvent(float x, float y)
	{
		if (mMouseLeftDown && !mMouseMiddleDown && !mMouseRightDown)
			MouseLeftDrag(x, y);
		else if (!mMouseLeftDown && !mMouseMiddleDown && mMouseRightDown)
			MouseRightDrag(x, y);
		else if (!mMouseLeftDown && mMouseMiddleDown && !mMouseRightDown)
			MouseMiddleDrag(x, y);

		mMouseX = x;
		mMouseY = y;
	}

	unsigned int Renderer::IsReady()
	{
		GLenum status =  glewInit(); //Init glew
		if(status != GLEW_OK)
			std::cout << "Error: could not initialize GLEW!" << std::endl;
		return status;
	}

	void Renderer::MouseLeftDrag(float x, float y)
	{
		float dx = x - mMouseX;
		float dy = y - mMouseY;
		float dPhi = dx * (PI / mWidth);
		float dTheta = dy * (PI / mHeight);
		mpCamera->Rotate(dPhi, -dTheta);
	}

	void Renderer::MouseMiddleDrag(float x, float y)
	{
		mpCamera->Move(-(x - mMouseX) / float(mWidth), (y - mMouseY) / float(mHeight));
	}

	void Renderer::MouseRightDrag(float x, float y)
	{
		mpCamera->Scale((y - mMouseY) / float(mHeight));
	}

}
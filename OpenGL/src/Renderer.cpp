#include "Renderer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Misc.h"
#include "Camera.h"
#include "GLOjbect.h"
#include "Scene.h"
#include "math/Lingal.h"
#include <iostream>

namespace OpenGL
{
	using namespace Math;

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
		mpScene = nullptr;
		mDrawWireFrame = true;
	}

	void Renderer::Init()
	{
		//GLCall(glEnable(GL_LINE_SMOOTH));
		//GLCall(glHint(GL_LINE_SMOOTH_HINT, GL_NICEST));
		GLCall(glEnable(GL_POLYGON_SMOOTH));
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

	void Renderer::SetScene(Scene *pScene)
	{
		mpScene = pScene;
	}

	void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
	{
		shader.Bind();
		va.Bind();
		ib.Bind();

		GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
	}

	void Renderer::DrawFaces(const DrawableObject& object) const
	{
		GLCall(glDisable(GL_BLEND));
		GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
		GLCall(glEnable(GL_POLYGON_OFFSET_FILL));
		GLCall(glPolygonOffset(1.0, 1.0));
		object.DrawFace(*this);
	}

	void Renderer::DrawWireFrame(const DrawableObject& object) const
	{
		GLCall(glDisable(GL_BLEND));
		GLCall(glDisable(GL_POLYGON_OFFSET_FILL));
		GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		GLCall(glLineWidth(0.5));
		object.DrawWireFrame(*this);
	}

	void Renderer::DrawFaces() const
	{
		GLCall(glDisable(GL_BLEND));
		GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
		GLCall(glEnable(GL_POLYGON_OFFSET_FILL));
		GLCall(glPolygonOffset(1.0, 1.0));
		const auto& drawObjects = mpScene->GetDrawObjects();
		for (DrawableObject* pObject : drawObjects)
		{
			pObject->DrawFace(*this);
		}
	}

	void Renderer::DrawWireFrame() const 
	{
		GLCall(glDisable(GL_BLEND));
		GLCall(glDisable(GL_POLYGON_OFFSET_FILL));
		GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		const auto& drawObjects = mpScene->GetDrawObjects();
		for (DrawableObject* pObject : drawObjects)
		{
			pObject->DrawWireFrame(*this);
		}
	}

	void Renderer::SetCamera(Camera* camera)
	{
		mpCamera = camera;
	}

	void Renderer::Clear() const
	{
		//GLCall(glClearColor(0.45f, 0.55f, 0.60f, 1.00f));
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

	void Renderer::DrawCoordinates() const
	{
		GLCall(glDisable(GL_DEPTH_TEST));
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE));
		GLCall(glLineWidth(2.0));
		Matrix4 model(1);
		Matrix4 view = mpCamera->GetViewMatrix();
		Matrix4 proj = mpCamera->GetProjMatrix();
		mCoords.mCoordShader.Bind();
		mCoords.mCoordShader.SetUniformMat4f("u_Model", model);
		mCoords.mCoordShader.SetUniformMat4f("u_View", view);
		mCoords.mCoordShader.SetUniformMat4f("u_Proj", proj);
		mCoords.mCoordShader.SetUniform4f("u_LineColor", 1.0f, 0.0f, 0.0f, 0.5f);
		mCoords.mVAX.Bind();
		mCoords.mIB.Bind();
		GLCall(glDrawElements(GL_LINES, mCoords.mIB.GetCount(), GL_UNSIGNED_INT, nullptr));
		mCoords.mIB.UnBind();
		mCoords.mVAX.UnBind();

		mCoords.mVAY.Bind();
		mCoords.mIB.Bind();
		mCoords.mCoordShader.SetUniform4f("u_LineColor", 0.0f, 1.0f, 0.0f, 0.5f);
		GLCall(glDrawElements(GL_LINES, mCoords.mIB.GetCount(), GL_UNSIGNED_INT, nullptr));
		mCoords.mIB.UnBind();
		mCoords.mVAY.UnBind();

		mCoords.mVAZ.Bind();
		mCoords.mIB.Bind();
		mCoords.mCoordShader.SetUniform4f("u_LineColor", 0.0f, 0.0f, 1.0f, 0.5f);
		GLCall(glDrawElements(GL_LINES, mCoords.mIB.GetCount(), GL_UNSIGNED_INT, nullptr));
		mCoords.mIB.UnBind();
		mCoords.mVAZ.UnBind();

		mCoords.mVAXZ.Bind();
		mCoords.mIBXZ.Bind();
		mCoords.mCoordShader.SetUniform4f("u_LineColor", 0.5f, 0.5f, 0.5f, 0.5f);
		GLCall(glDrawElements(GL_LINES, mCoords.mIBXZ.GetCount(), GL_UNSIGNED_INT, nullptr));
		mCoords.mIBXZ.UnBind();
		mCoords.mVAXZ.UnBind();

		mCoords.mCoordShader.UnBind();
		GLCall(glEnable(GL_DEPTH_TEST));
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
		if (!mLockCamera)
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

	}

	void Renderer::CursorEvent(float x, float y)
	{
		if (mMouseLeftDown && !mMouseMiddleDown && !mMouseRightDown)
			MouseLeftDrag(x, y);
		else if (!mMouseLeftDown && !mMouseMiddleDown && mMouseRightDown)
			MouseRightDrag(x, y);
		else if (!mMouseLeftDown && mMouseMiddleDown && !mMouseRightDown)
			MouseMiddleDrag(x, y);
		else if (mMouseLeftDown && !mMouseMiddleDown && mMouseRightDown)
			MouseLeftRightDrag(x, y);

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
		if (!mLockCamera)
		{
			float dx = x - mMouseX;
			float dy = y - mMouseY;
			float dPhi = dx * (PI / mWidth);
			float dTheta = dy * (PI / mHeight);
			mpCamera->Rotate(dPhi, -dTheta);
		}
	}

	void Renderer::MouseMiddleDrag(float x, float y)
	{
		if (!mLockCamera)
			mpCamera->Move(-(x - mMouseX) / float(mWidth), (y - mMouseY) / float(mHeight));
	}

	void Renderer::MouseLeftRightDrag(float x, float y)
	{
		if (!mLockCamera)
			mpCamera->Move(-(x - mMouseX) / float(mWidth), (y - mMouseY) / float(mHeight));
	}

	void Renderer::MouseRightDrag(float x, float y)
	{
		if(!mLockCamera)
			mpCamera->Scale((y - mMouseY) / float(mHeight));
	}

}
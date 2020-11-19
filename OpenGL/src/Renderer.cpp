#include "Renderer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Misc.h"
#include "Camera.h"
#include "GLOjbect.h"
#include "Scene.h"
#include "math/Lingal.h"
#include "DrawableObject.h"
#include <iostream>

namespace OpenGL
{
	using namespace Math;

	Renderer::Renderer()
	{
		mWidth = DEFAULT_RESOLUTION_X;
		mHeight = DEFAULT_RESOLUTION_Y;

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

	void Renderer::DrawObjectId(const DrawableObject& obejct) const
	{
		obejct.DrawObjectId(*this);
	}

	void Renderer::DrawObjectsId(Scene& scene) const
	{
		GLCall(glDisable(GL_BLEND));
		GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
		GLCall(glEnable(GL_POLYGON_OFFSET_FILL));
		GLCall(glPolygonOffset(1.0, 1.0));
		for (auto it : scene.GetDrawObjects()) 
		{
			it->DrawObjectId(*this);
		}
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
		GLCall(glDisable(GL_BLEND));
	}

	void Renderer::SetMousePoseition(float x, float y)
	{
		mMouseX = x;
		mMouseY = y;
	}

	void Renderer::SetCamera(Camera* pCamera)
	{
		mpCamera = pCamera;
	}

	unsigned int Renderer::GetObjectId(int x, int y)
	{
		return 0;
	}



	void Renderer::TestPick(const DrawableObject &object)
	{
		GLCall(glDisable(GL_BLEND));
		GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
		GLCall(glEnable(GL_POLYGON_OFFSET_FILL));
		GLCall(glPolygonOffset(1.0, 1.0));
		object.DrawObjectId(*this);
		glFlush();
		glFinish();
	}



	unsigned int Renderer::IsReady()
	{
		GLenum status =  glewInit(); //Init glew
		if(status != GLEW_OK)
			std::cout << "Error: could not initialize GLEW!" << std::endl;
		return status;
	}


	void Renderer::MousePick()
	{

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		unsigned char data[4];
		glReadPixels((int)mMouseX, mHeight - int(mMouseY) - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

		// Convert the color back to an integer ID
		int pickedID =
			data[0] +
			data[1] * 256 +
			data[2] * 256 * 256;

		std::cout << mMouseX << " " << mMouseY << std::endl;
		if (pickedID == 0) { // Full white, must be the background !
			std::cout << "background" << std::endl;
		}
		else {

			std::cout << "mesh " << pickedID - 1000 << std::endl;

		}
	}


}
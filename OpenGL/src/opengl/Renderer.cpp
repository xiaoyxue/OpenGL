#include "Renderer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Misc.h"
#include "visual/Camera.h"
#include "GLOjbect.h"
#include "Scene.h"
#include "math/Lingal.h"
#include "DrawableObject.h"
#include "math/AABB.h"
#include "DrawQuad.h"
#include <iostream>

namespace OpenGL
{
	using namespace Math;

	//Renderer::Renderer()
	//{
	//	mWidth = DEFAULT_RESOLUTION_X;
	//	mHeight = DEFAULT_RESOLUTION_Y;

	//}

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
		GLCall(glViewport(0, 0, mWidth, mHeight));
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
		//GLCall(glDisable(GL_BLEND));
		EnableBlend();
		GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
		GLCall(glEnable(GL_POLYGON_OFFSET_FILL));
		GLCall(glPolygonOffset(1.0, 1.0));
		object.DrawFace(*this);
	}

	void Renderer::DrawFaces(const Scene& scene) const
	{
		//GLCall(glDisable(GL_BLEND));
		//EnableBlend();
		//GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
		//GLCall(glEnable(GL_POLYGON_OFFSET_FILL));
		//GLCall(glPolygonOffset(1.0, 1.0));
		for (auto it : scene.GetDrawObjects())
		{
			it->DrawFace(*this);
		}
	}

	void Renderer::DrawWireFrame(const DrawableObject& object) const
	{
		GLCall(glDisable(GL_BLEND));
		GLCall(glDisable(GL_POLYGON_OFFSET_FILL));
		GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		GLCall(glLineWidth(0.5));
		object.DrawWireFrame(*this);
	}

	void Renderer::DrawWireFrame(const Scene& scene) const
	{
		GLCall(glDisable(GL_BLEND));
		GLCall(glDisable(GL_POLYGON_OFFSET_FILL));
		GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		GLCall(glLineWidth(0.5));
		for (auto it : scene.GetDrawObjects())
		{
			it->DrawWireFrame(*this);
		}
	}

	void Renderer::DrawObjectId(const DrawableObject& obejct) const
	{
		GLCall(glDisable(GL_BLEND));
		GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
		GLCall(glEnable(GL_POLYGON_OFFSET_FILL));
		GLCall(glPolygonOffset(1.0, 1.0));
		obejct.DrawObjectId(*this);
	}

	void Renderer::DrawObjectId(const Scene& scene) const
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

	void Renderer::DrawBBox(const DrawableObject& object, const Vec4& lineColor) const
	{
		BBoxDrawer bboxDrawer(object.GetBBox());
		GLCall(glDisable(GL_DEPTH_TEST));
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE));
		GLCall(glLineWidth(1.5));
		Matrix4 model(1);
		Matrix4 view = mpCamera->GetViewMatrix();
		Matrix4 proj = mpCamera->GetProjMatrix();
		bboxDrawer.mBBoxShader.Bind();
		bboxDrawer.mBBoxShader.SetUniformMat4f("u_Model", model);
		bboxDrawer.mBBoxShader.SetUniformMat4f("u_View", view);
		bboxDrawer.mBBoxShader.SetUniformMat4f("u_Proj", proj);
		bboxDrawer.mBBoxShader.SetUniform4f("u_LineColor", lineColor[0], lineColor[1], lineColor[2], lineColor[3]);
		bboxDrawer.mVa.Bind();
		bboxDrawer.mIb.Bind();
		GLCall(glDrawElements(GL_LINES, bboxDrawer.mIb.GetCount(), GL_UNSIGNED_INT, nullptr));
		bboxDrawer.mIb.UnBind();
		bboxDrawer.mVa.UnBind();
		bboxDrawer.mBBoxShader.UnBind();
	}

	void Renderer::DrawBBox(const Scene& scene, const Vec4& lineColor) const
	{
		BBoxDrawer bboxDrawer(scene.GetBBox());
		GLCall(glDisable(GL_DEPTH_TEST));
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE));
		GLCall(glLineWidth(1.0));
		Matrix4 model(1);
		Matrix4 view = mpCamera->GetViewMatrix();
		Matrix4 proj = mpCamera->GetProjMatrix();
		bboxDrawer.mBBoxShader.Bind();
		bboxDrawer.mBBoxShader.SetUniformMat4f("u_Model", model);
		bboxDrawer.mBBoxShader.SetUniformMat4f("u_View", view);
		bboxDrawer.mBBoxShader.SetUniformMat4f("u_Proj", proj);
		bboxDrawer.mBBoxShader.SetUniform4f("u_LineColor", lineColor[0], lineColor[1], lineColor[2], lineColor[3]);
		bboxDrawer.mVa.Bind();
		bboxDrawer.mIb.Bind();
		GLCall(glDrawElements(GL_LINES, bboxDrawer.mIb.GetCount(), GL_UNSIGNED_INT, nullptr));
		mCoords.mIb.UnBind();
		mCoords.mVaX.UnBind();
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

	void Renderer::EnableBlend() const
	{
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	}

	void Renderer::DisableBlend() const
	{
		GLCall(glDisable(GL_BLEND));
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
		mCoords.mVaX.Bind();
		mCoords.mIb.Bind();
		GLCall(glDrawElements(GL_LINES, mCoords.mIb.GetCount(), GL_UNSIGNED_INT, nullptr));
		mCoords.mIb.UnBind();
		mCoords.mVaX.UnBind();

		mCoords.mVaY.Bind();
		mCoords.mIb.Bind();
		mCoords.mCoordShader.SetUniform4f("u_LineColor", 0.0f, 1.0f, 0.0f, 0.5f);
		GLCall(glDrawElements(GL_LINES, mCoords.mIb.GetCount(), GL_UNSIGNED_INT, nullptr));
		mCoords.mIb.UnBind();
		mCoords.mVaY.UnBind();

		mCoords.mVaZ.Bind();
		mCoords.mIb.Bind();
		mCoords.mCoordShader.SetUniform4f("u_LineColor", 0.0f, 0.0f, 1.0f, 0.5f);
		GLCall(glDrawElements(GL_LINES, mCoords.mIb.GetCount(), GL_UNSIGNED_INT, nullptr));
		mCoords.mIb.UnBind();
		mCoords.mVaZ.UnBind();

		mCoords.mVaXZ.Bind();
		mCoords.mIBXZ.Bind();
		mCoords.mCoordShader.SetUniform4f("u_LineColor", 0.5f, 0.5f, 0.5f, 0.5f);
		GLCall(glDrawElements(GL_LINES, mCoords.mIBXZ.GetCount(), GL_UNSIGNED_INT, nullptr));
		mCoords.mIBXZ.UnBind();
		mCoords.mVaXZ.UnBind();

		mCoords.mCoordShader.UnBind();
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDisable(GL_BLEND));
	}

	void Renderer::DrawBoarder(int w) const
	{
		BoarderDrawer boarderDrawer;
		EnableBlend();
		DisableDepthTest();
		boarderDrawer.Draw(w, Vec2i(mWidth, mHeight), this);
	}

	void Renderer::DrawGbuffer(const DrawableObject& object, FrameBuffer& frameBuffer) const
	{
		object.DrawToGbuffer(*this, frameBuffer);
	}


	void Renderer::DrawGbuffer(const Scene& scene, FrameBuffer& frameBuffer) const
	{
		//GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
		//GLCall(glEnable(GL_POLYGON_OFFSET_FILL));
		//GLCall(glPolygonOffset(1.0, 1.0));
		for (auto it : scene.GetDrawObjects())
		{
			it->DrawToGbuffer(*this, frameBuffer);
		}
	}

	void Renderer::DrawToFrameBuffer(const DrawableObject& object, FrameBuffer& frameBuffer) const
	{
		object.DrawToFrameBuffer(*this, frameBuffer);
	}

	void Renderer::DrawToFrameBuffer(const Scene& scene, FrameBuffer& frameBuffer) const
	{
		for (auto it : scene.GetDrawObjects())
		{
			it->DrawToFrameBuffer(*this, frameBuffer);
		}
	}

	void Renderer::DebugDraw(DrawQuad& quad, Texture2D& texture) const
	{
		quad.AddTexture(&texture);
		quad.DrawFace(*this);
	}

	void Renderer::DebugDraw(DrawQuad& quad) const
	{
		quad.DebugDraw(*this);
	}

	void Renderer::SetCamera(Camera* pCamera)
	{
		mpCamera = pCamera;
	}

	void Renderer::SetSSAO(SSAO* pSSAO)
	{
		mpSSAO = pSSAO;
	}

	unsigned int Renderer::IsReady()
	{
		GLenum status =  glewInit(); //Init glew
		if(status != GLEW_OK)
			std::cout << "Error: could not initialize GLEW!" << std::endl;
		return status;
	}


}
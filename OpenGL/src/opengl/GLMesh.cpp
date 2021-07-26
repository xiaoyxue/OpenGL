#include "GLMesh.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "ObjMesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "visual/Camera.h"
#include "ssao/SSAO.h"
#include <sstream>

namespace OpenGL
{
	GLMesh::GLMesh()
	{

	}

	GLMesh::~GLMesh()
	{

	}

	void GLMesh::DrawFace(const Renderer& renderer) const
	{
		std::stringstream ss;
		auto& shader = mShaders["Face"];
		renderer.EnableDepthTest();
		shader->Bind();
		Matrix4 proj = renderer.GetCamera()->GetProjMatrix();
		Matrix4 view = renderer.GetCamera()->GetViewMatrix();
		Matrix4 model = LocalToWorld.GetMatrix();
		Vec3 camPosition = renderer.GetCamera()->GetPosition();
		shader->SetUniformMat4f("u_Model", model);
		shader->SetUniformMat4f("u_View", view);
		shader->SetUniformMat4f("u_Proj", proj);
		shader->SetUniform3f("cameraPosition", camPosition[0], camPosition[1], camPosition[2]);
		for (int i = 0; i < mTextures.size(); ++i)
		{
			ss.clear();
			ss << i;
			std::string textureName = "u_Texture" + ss.str();
			shader->SetUniform1i(textureName, i);
			mTextures[i]->Bind(i);
		}
		SSAO* pSSAO = renderer.GetSSAO();
		shader->SetUniform1i("ssao", 5); pSSAO->mpSSAO_Color->Bind(5);
		renderer.Draw(*mpVAO, *mpIBO, *shader);
		renderer.DisableDepthTest();
		for (int i = 0; i < mTextures.size(); ++i)
		{
			mTextures[i]->UnBind();
		}
		shader->UnBind();
	}

	void GLMesh::DrawToGbuffer(const Renderer& renderer, const FrameBuffer& frameBuffer) const
	{
		frameBuffer.Bind();

		auto& shader = mShaders["Gbuffer"];
		renderer.EnableDepthTest();
		shader->Bind();
		Matrix4 proj = renderer.GetCamera()->GetProjMatrix();
		Matrix4 view = renderer.GetCamera()->GetViewMatrix();
		Matrix4 model = LocalToWorld.GetMatrix();
		shader->SetUniformMat4f("Model", model);
		shader->SetUniformMat4f("View", view);
		shader->SetUniformMat4f("Proj", proj);
		Vec3 cameraPosition = renderer.GetCamera()->GetPosition();
		shader->SetUniform3f("cameraPosition", cameraPosition[0], cameraPosition[1], cameraPosition[2]);
		renderer.Draw(*mpVAO, *mpIBO, *shader);
		renderer.DisableDepthTest();
		shader->UnBind();

		frameBuffer.UnBind();
	}

	void GLMesh::DrawWireFrame(const Renderer& renderer) const
	{
		auto& shader = mShaders["WireFrame"];
		renderer.EnableDepthTest();
		shader->Bind();
		Matrix4 proj = renderer.GetCamera()->GetProjMatrix();
		Matrix4 view = renderer.GetCamera()->GetViewMatrix();
		Matrix4 model = LocalToWorld.GetMatrix();
		shader->SetUniformMat4f("u_Model", model);
		shader->SetUniformMat4f("u_View", view);
		shader->SetUniformMat4f("u_Proj", proj);
		shader->SetUniform4f("u_LineColor", 0.3f, 0.3f, 0.3f, 0.0f);
		renderer.Draw(*mpVAO, *mpIBO, *shader);
		renderer.DisableDepthTest();
		shader->UnBind();
	}

	void GLMesh::DrawObjectId(const Renderer& renderer) const
	{
		auto& shader = mSelectorShader;
		renderer.EnableDepthTest();
		shader.Bind();
		Matrix4 proj = renderer.GetCamera()->GetProjMatrix();
		Matrix4 view = renderer.GetCamera()->GetViewMatrix();
		Matrix4 model = LocalToWorld.GetMatrix();
		shader.SetUniformMat4f("u_Model", model);
		shader.SetUniformMat4f("u_View", view);
		shader.SetUniformMat4f("u_Proj", proj);
		int r = ((mObjectId + 1) & 0x000000FF) >> 0;
		int g = ((mObjectId + 1) & 0x0000FF00) >> 8;
		int b = ((mObjectId + 1) & 0x00FF0000) >> 16;
		shader.SetUniform4f("u_Id", r / 255.f, g / 255.f, b / 255.f, 1.0f);
		renderer.Draw(*mpVAO, *mpIBO, shader);
		renderer.DisableDepthTest();
		shader.UnBind();
	}

	BBox GLMesh::GetBBox() const
	{
		return  (Transform(mTraceMatrix) * LocalToWorld)(mpObjMesh->mBBox); 
	}

	void GLMesh::AddMesh(const std::string& filename)
	{
		mpObjMesh = std::make_unique<ObjMesh>();
		mpObjMesh->Load(filename);
		mpVAO = std::make_unique<VertexArray>();
		mpVBO = std::make_unique<VertexBuffer>();
		mpVBO->SetData(&(mpObjMesh->mGLVertices[0]), mpObjMesh->mGLVertices.size() * sizeof(float));
		mpIBO = std::make_unique<IndexBuffer>();
		mpIBO->SetData(&(mpObjMesh->mGLIndices[0]), mpObjMesh->mGLIndices.size());
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);
		layout.Push<float>(3);
		layout.Push<float>(3);
		mpVAO->AddBuffer(*mpVBO, layout);
		mpVAO->UnBind();
		mpVBO->UnBind();
		mpIBO->UnBind();
		LocalToWorld = Inverse(Transform::Translate(mpObjMesh->mBBox.Center()));
		WorldToLocal = Inverse(LocalToWorld);

	}

	void GLMesh::AddShader(const std::string &shaderName, std::shared_ptr<Shader> shader)
	{
		mShaders[shaderName] = shader;
	}

}
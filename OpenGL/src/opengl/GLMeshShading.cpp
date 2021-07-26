#include "GLMeshShading.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "ObjMesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "visual/Camera.h"
#include "ssao/SSAO.h"
#include <map>
#include <sstream>

namespace OpenGL {

	GLMeshShading::GLMeshShading()
		: GLMesh()
	{

	}

	void GLMeshShading::DrawFace(const Renderer& renderer) const
	{
		std::stringstream ss;
		auto& shader = mShaders["Face"];
		renderer.EnableDepthTest();
		shader->Bind();
		Matrix4 proj = renderer.GetCamera()->GetProjMatrix();
		Matrix4 view = renderer.GetCamera()->GetViewMatrix();
		Matrix4 model = LocalToWorld.GetMatrix();
		shader->SetUniformMat4f("u_Model", model);
		shader->SetUniformMat4f("u_View", view);
		shader->SetUniformMat4f("u_Proj", proj);
		Vec3 camPosition = renderer.GetCamera()->GetPosition();
		shader->SetUniform3f("cameraPosition", camPosition[0], camPosition[1], camPosition[2]);
		//shader->SetUniform1i("faceTextureSampler", 0);
		shader->SetUniform1i("environmentTextureSampler", 0); mTextureMapping["envMap"]->Bind(0);
		shader->SetUniform1i("brdfLUTSampler", 1); mTextureMapping["brdfLUT"]->Bind(1);
		SSAO* pSSAO = renderer.GetSSAO();
		shader->SetUniform1i("ssao", 10); pSSAO->mpSSAO_Color->Bind(10);
		shader->SetUniform1f("roughness", 0.55);
		shader->SetUniform3f("F0", 0.999, 0.999, 0.999);
		renderer.Draw(*mpVAO, *mpIBO, *shader);
		renderer.DisableDepthTest();

		mTextureMapping["envMap"]->UnBind();
		mTextureMapping["brdfLUT"]->UnBind();
		shader->UnBind();
	}

	void GLMeshShading::DrawToGbuffer(const Renderer& renderer, const FrameBuffer& frameBuffer) const
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
		renderer.Draw(*mpVAO, *mpIBO, *shader);
		renderer.DisableDepthTest();
		shader->UnBind();

		frameBuffer.UnBind();
	}

	void GLMeshShading::DebugDraw(const Renderer& renderer) const
	{
		auto shader = &mDebugShader;
		renderer.EnableDepthTest();
		shader->Bind();
		Matrix4 proj = renderer.GetCamera()->GetProjMatrix();
		Matrix4 view = renderer.GetCamera()->GetViewMatrix();
		Matrix4 model = LocalToWorld.GetMatrix();
		shader->SetUniformMat4f("u_Model", model);
		shader->SetUniformMat4f("u_View", view);
		shader->SetUniformMat4f("u_Proj", proj);
		renderer.Draw(*mpVAO, *mpIBO, *shader);
		renderer.DisableDepthTest();
		shader->UnBind();
	}



}
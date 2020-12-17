#include "GLBackground.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "ObjMesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "visual/Camera.h"


namespace OpenGL
{
	using namespace Math;
	GLBackground::GLBackground()
	{
		mpVAO = std::make_unique<VertexArray>();
		mpVBO = std::make_unique<VertexBuffer>();
		mpIBO = std::make_unique<IndexBuffer>();
		mpVBO->SetData(&(mPoints[0]), 32 * sizeof(float));
		mpIBO->SetData(&(mIndex[0]), 6);
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);
		mpVAO->AddBuffer(*mpVBO, layout);
		mpVAO->UnBind();
		mpVBO->UnBind();
		mpIBO->UnBind();
	}

	GLBackground::~GLBackground()
	{

	}

	void GLBackground::DrawFace(const Renderer& renderer) const
	{
		auto& shader = mShaders["Background"];
		renderer.EnableDepthTest();
		shader->Bind();
		Matrix4 proj = renderer.GetCamera()->GetProjMatrix();
		Matrix4 view = renderer.GetCamera()->GetViewMatrix();
		Matrix4 model(1);
		shader->SetUniformMat4f("u_Model", model);
		shader->SetUniformMat4f("u_View", view);
		shader->SetUniformMat4f("u_Proj", proj);
		shader->SetUniform1i("u_Texture", 0); mpTexture->Bind(0);
		renderer.Draw(*mpVAO, *mpIBO, *shader);
		renderer.DisableDepthTest();
		mpTexture->UnBind();
		shader->UnBind();
	}

	Math::BBox GLBackground::GetBBox() const
	{
		return BBox();
	}

	void GLBackground::AddShader(const std::string& shaderName, std::shared_ptr<Shader> shader)
	{
		mShaders[shaderName] = shader;
	}

}


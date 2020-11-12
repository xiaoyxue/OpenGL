#include "GLMesh.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "ObjMesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "Camera.h"

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
		auto shader = mShaders["Face"];
		renderer.EnableDepthTest();
		shader->Bind();
		//glm::mat4 proj = renderer.GetCamera()->GetProjMatrix();
		//glm::mat4 view = renderer.GetCamera()->GetViewMatrix();
		//glm::mat4 model = glm::mat4(1.f);
		Matrix4 proj = renderer.GetCamera()->GetProjMatrix();
		Matrix4 view = renderer.GetCamera()->GetViewMatrix();
		Matrix4 model = Matrix4(1.f);
		shader->SetUniformMat4f("u_Model", model);
		shader->SetUniformMat4f("u_View", view);
		shader->SetUniformMat4f("u_Proj", proj);
		renderer.Draw(*mpVAO, *mpIBO, *shader);
		renderer.DisableDepthTest();
	}


	void GLMesh::DrawWireFrame(const Renderer& renderer) const
	{
		auto shader = mShaders["WireFrame"];
		renderer.EnableDepthTest();
		shader->Bind();
		//glm::mat4 proj = renderer.GetCamera()->GetProjMatrix();
		//glm::mat4 view = renderer.GetCamera()->GetViewMatrix();
		//glm::mat4 model = glm::mat4(1.f);
		Matrix4 proj = renderer.GetCamera()->GetProjMatrix();
		Matrix4 view = renderer.GetCamera()->GetViewMatrix();
		Matrix4 model = Matrix4(1.f);
		shader->SetUniformMat4f("u_Model", model);
		shader->SetUniformMat4f("u_View", view);
		shader->SetUniformMat4f("u_Proj", proj);
		shader->SetUniform4f("u_LineColor", 0.3f, 0.3f, 0.3f, 0.0f);
		renderer.Draw(*mpVAO, *mpIBO, *shader);
		renderer.DisableDepthTest();
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
		layout.Push<float>(2);
		layout.Push<float>(3);
		layout.Push<float>(3);
		mpVAO->AddBuffer(*mpVBO, layout);
		mpVAO->UnBind();
		mpVBO->UnBind();
		mpIBO->UnBind();
	}

	void GLMesh::AddShader(const std::string &shaderName, std::shared_ptr<Shader> shader)
	{
		mShaders[shaderName] = shader;
	}

}
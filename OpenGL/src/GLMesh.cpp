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

	void GLMesh::Draw(const Renderer& renderer, const Shader& shader)
	{
		renderer.EnableDepthTest();
		mShader.Bind();
		glm::mat4 proj = renderer.GetCamera()->GetProjMatrix();
		glm::mat4 view = renderer.GetCamera()->GetViewMatrix();
		glm::mat4 model = glm::mat4(1.f);
		mShader.SetUniformMat4f("u_Model", model);
		mShader.SetUniformMat4f("u_View", view);
		mShader.SetUniformMat4f("u_Proj", proj);
		renderer.Draw(*mpVAO, *mpIBO, mShader);
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
		mpVAO->AddBuffer(*mpVBO, layout);
		mpVAO->UnBind();
		mpVBO->UnBind();
		mpIBO->UnBind();
	}

	void GLMesh::SetShader(const Shader& shader)
	{
		mShader = shader;
	}

}
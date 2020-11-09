#include "GLMesh.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "ObjMesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"

namespace OpenGL
{


	GLMesh::GLMesh()
	{

	}

	GLMesh::~GLMesh()
	{

	}

	void GLMesh::Draw(const Renderer& renderer, const Shader& shader) const
	{
		renderer.EnableDepthTest();

		renderer.Draw(*mpVAO, *mpIBO, shader);
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
		mpVAO->AddBuffer(*mpVBO, layout);
		mpVAO->UnBind();
		mpVBO->UnBind();
		mpIBO->UnBind();
	}

}
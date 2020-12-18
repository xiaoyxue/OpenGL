#include "GLPointCloud.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "ObjMesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "visual/Camera.h"
#include <sstream>
namespace OpenGL
{

	void GLPointCloud::DrawFace(const Renderer& renderer) const
	{
		std::stringstream ss;
		auto& shader = mShaders["Point"];
		renderer.EnableDepthTest();
		shader->Bind();
		Matrix4 proj = renderer.GetCamera()->GetProjMatrix();
		Matrix4 view = renderer.GetCamera()->GetViewMatrix();
		Matrix4 model = LocalToWorld.GetMatrix();
		shader->SetUniformMat4f("u_Model", model);
		shader->SetUniformMat4f("u_View", view);
		shader->SetUniformMat4f("u_Proj", proj);
		renderer.Draw<DrawModel::POINT>(*mpVAO, *mpIBO, *shader);
		renderer.DisableDepthTest();
		shader->UnBind();
	}

	Math::BBox GLPointCloud::GetBBox() const
	{
		return  (LocalToWorld)(mpPointCloud->mBBox);
	}

	void GLPointCloud::AddPoints(const std::string& filename)
	{
		mpPointCloud = std::make_unique<PointCloud>();
		mpPointCloud->Load(filename);
		mpVAO = std::make_unique<VertexArray>();
		mpVBO = std::make_unique<VertexBuffer>();
		mpVBO->SetData(&(mpPointCloud->mVertices[0]), mpPointCloud->mVertices.size() * sizeof(float));
		mpIBO = std::make_unique<IndexBuffer>();
		mpIBO->SetData(&(mpPointCloud->mGLIndices[0]), mpPointCloud->mGLIndices.size());
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		mpVAO->AddBuffer(*mpVBO, layout);
		mpVAO->UnBind();
		mpVBO->UnBind();
		mpIBO->UnBind();
		LocalToWorld = Inverse(Transform::Translate(mpPointCloud->mBBox.Center()));
		WorldToLocal = Inverse(LocalToWorld);
	}

	void GLPointCloud::AddShader(const std::string& shaderName, std::shared_ptr<Shader> shader)
	{
		mShaders[shaderName] = shader;
	}

}


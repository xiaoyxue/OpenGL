#include "GLMeshShading.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "ObjMesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "visual/Camera.h"
#include <sstream>

namespace OpenGL {

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
		shader->SetUniform1i("environmentTextureSampler", 0);
		shader->SetUniform1i("brdfLUTSampler", 1);
		for (int i = 0; i < mTextures.size(); ++i)
		{
			mTextures[i]->Bind(i);
		}

		shader->SetUniform1f("roughness", 0.55);
		shader->SetUniform3f("F0", 0.999, 0.999, 0.999);

		//for (int i = 0; i < mTextures.size(); ++i)
		//{
		//	ss.clear();
		//	ss << i;
		//	std::string textureName = "u_Texture" + ss.str();
		//	shader->SetUniform1i(textureName, i);
		//	mTextures[i]->Bind(i);
		//}
		renderer.Draw(*mpVAO, *mpIBO, *shader);
		renderer.DisableDepthTest();
		for (int i = 0; i < mTextures.size(); ++i)
		{
			mTextures[i]->UnBind();
		}
		shader->UnBind();
	}

}
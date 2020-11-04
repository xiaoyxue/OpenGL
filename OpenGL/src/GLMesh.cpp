#include "GLMesh.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"

namespace OpenGL
{


	void GLMesh::Draw(const Renderer& renderer, const Shader& shader) const
	{
		renderer.EnableDepthTest();

		renderer.Draw(*mVAO, *mIBO, shader);
		renderer.DisableDepthTest();
	}

}
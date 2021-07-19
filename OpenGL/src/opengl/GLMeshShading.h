#pragma once
#include "GLMesh.h"

namespace OpenGL
{
	using namespace Math;

	class GLMeshShading: public GLMesh {
		void DrawFace(const Renderer& renderer) const override;
	};
}
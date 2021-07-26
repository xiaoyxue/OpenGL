#pragma once
#include "GLMesh.h"
#include <random>

namespace OpenGL
{
	using namespace Math;

	class GLMeshShading: public GLMesh {
	public:
		GLMeshShading();
		void DrawFace(const Renderer& renderer) const override;
		void DrawToGbuffer(const Renderer& renderer, const FrameBuffer& frameBuffer) const override;
		void DebugDraw(const Renderer& renderer) const override;

	};
}
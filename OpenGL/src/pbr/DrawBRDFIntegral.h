#pragma once
#include "opengl/DrawableObject.h"
#include "opengl/FrameBuffer.h"

namespace PBR
{
	using namespace OpenGL;
	class BRDFIntegral : public DrawableObject
	{
	private:
		float mPoints[32] =
		{	// position, normal, textureCoord
			// set z as a very small value to be always the background
			-1, -1, -100,   0, 0, 1,   0, 0,
			 1, -1, -100,   0, 0, 1,   1, 0,
			 1,  1, -100,   0, 0, 1,   1, 1,
			-1,  1, -100,   0, 0, 1,   0, 1
		};
		unsigned int mIndex[6] = { 0, 1, 2, 0, 2, 3 };
		std::unique_ptr<VertexBuffer> mpVBO;
		std::unique_ptr<IndexBuffer> mpIBO;
		std::unique_ptr<VertexArray> mpVAO;
		std::unique_ptr<FrameBuffer> mpFrameBuffer;
		//std::unique_ptr<RenderBuffer> mpRenderBuffer;
		Texture2D* mpTexture;
		mutable std::unordered_map<std::string, std::shared_ptr<Shader>> mShaders;
		unsigned int mHandler;
	public:
		BRDFIntegral();
		virtual ~BRDFIntegral();
		void DrawFace(const Renderer& renderer) const override;
		void AddShader(const std::string& shaderName, std::shared_ptr<Shader> shader);
		void AddTexture(Texture2D* texture) { mpTexture = texture; }
		BBox GetBBox() const { return BBox(); }
	};
}
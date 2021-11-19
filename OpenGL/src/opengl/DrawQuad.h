#pragma once
#include "DrawableObject.h"
#include "glm/glm.hpp"
namespace OpenGL
{
	class DrawQuad : public DrawableObject
	{
	private:
		float mPoints[32] =
		{	// position, normal, textureCoord
			// set z as a very small value to be always the background
			-1, -1, 0,   0, 0, 1,   0, 0,
			 1, -1, 0,   0, 0, 1,   1, 0,
			 1,  1, 0,   0, 0, 1,   1, 1,
			-1,  1, 0,   0, 0, 1,   0, 1
		};
		unsigned int mIndex[6] = { 0, 1, 2, 0, 2, 3 };
		std::unique_ptr<VertexBuffer> mpVBO;
		std::unique_ptr<IndexBuffer> mpIBO;
		std::unique_ptr<VertexArray> mpVAO;
		Texture2D* mpTexture;
		mutable std::unordered_map<std::string, Texture2D*> mTextures;
		mutable std::unordered_map<std::string, std::shared_ptr<Shader>> mShaders;
		std::vector<Vec3> mSample;
		std::vector<Vec3> ssaoKernel;
		std::vector<glm::vec3> ssaoNoise;
		std::shared_ptr<Texture2D> mpNoiseTexture;
	public:
		DrawQuad();
		virtual ~DrawQuad();
		void DrawFace(const Renderer& renderer) const override;
		BBox GetBBox() const override;
		void AddShader(const std::string& shaderName, std::shared_ptr<Shader> shader);
		void AddTexture(Texture2D* pTexture) { mpTexture = pTexture; }
		void SetTexture(const std::string & textureName, Texture2D* pTexture) { mTextures[textureName] = pTexture; }
		void DrawSSAO(const Renderer& renderer, const SSAO& ssaoBuffer) const override;
		void DrawToFrameBuffer(const Renderer& renderer, const FrameBuffer& frameBuffer) const override;
		void GenerateSamples(int n);
		void DebugDraw(const Renderer& renderer) const;
	};
}
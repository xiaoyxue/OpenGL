#pragma once
#include "DrawableObject.h"
#include "Shader.h"
#include "ForwardDecl.h"
#include "OpenGL.h"
#include <iostream>
#include <vector>
#include <unordered_map>

namespace OpenGL 
{
	using namespace Math;
	class GLMesh: public DrawableObject
	{
	protected:
		std::unique_ptr<VertexBuffer> mpVBO;
		std::unique_ptr<IndexBuffer> mpIBO;
		std::unique_ptr<VertexArray> mpVAO;
		std::unique_ptr<ObjMesh> mpObjMesh;
		mutable std::map<std::string, Texture2D*> mTextureMapping;
		std::vector<Texture2D*> mTextures;
		mutable std::unordered_map<std::string, std::shared_ptr<Shader>> mShaders;
	public:
		GLMesh();
		virtual ~GLMesh();
		
		virtual void DrawFace(const Renderer& renderer) const override;
		void DrawWireFrame(const Renderer& renderer) const override;
		void DrawObjectId(const Renderer& renderer) const override;
		void DrawToGbuffer(const Renderer& renderer, const FrameBuffer& frameBuffer) const override;
		BBox GetBBox() const override;

		void AddTexture(Texture2D* texture) { mTextures.push_back(texture); }
		void SetTexture(const std::string& textureName, Texture2D* pTexture) { mTextureMapping[textureName] = pTexture; }
		void AddMesh(const std::string& filename);
		void AddShader(const std::string& shaderName, std::shared_ptr<Shader> shader);
	};
}
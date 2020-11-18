#pragma once
#include "DrawableObject.h"
#include "Shader.h"
#include "ForwardDecl.h"
#include <iostream>
#include <vector>
#include <unordered_map>

namespace OpenGL 
{
	using namespace Math;
	class GLMesh: public DrawableObject
	{
	private:
		std::unique_ptr<VertexBuffer> mpVBO;
		std::unique_ptr<IndexBuffer> mpIBO;
		std::unique_ptr<VertexArray> mpVAO;
		std::vector<Texture*> mTextures;
		std::unique_ptr<ObjMesh> mpObjMesh;
		mutable std::unordered_map<std::string, std::shared_ptr<Shader>> mShaders;
	public:
		GLMesh();
		virtual ~GLMesh();
		
		void DrawFace(const Renderer& renderer) const override;
		void DrawWireFrame(const Renderer& renderer) const override;
		void DrawObjectId(const Renderer& renderer) const override;
		BBox GetBBox() const override;

		void AddTexture(Texture* texture) { mTextures.push_back(texture); }
		void AddMesh(const std::string& filename);
		void AddShader(const std::string& shaderName, std::shared_ptr<Shader> shader);
	};
}
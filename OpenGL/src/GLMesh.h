#pragma once
#include "GLOjbect.h"
#include "Shader.h"
#include "ForwardDecl.h"
#include <iostream>
#include <vector>
#include <unordered_map>

namespace OpenGL 
{
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

		void AddTexture(Texture* texture) { mTextures.push_back(texture); }
		void AddMesh(const std::string& filename);
		void AddShader(const std::string& shaderName, std::shared_ptr<Shader> shader);
	};
}
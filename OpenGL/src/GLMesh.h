#pragma once
#include "GLOjbect.h"
#include <iostream>
#include <vector>
#include "Shader.h"
#include <unordered_map>

namespace OpenGL 
{
	class Renderer;
	class Shader;
	class Texture;
	class VertexBuffer;
	class IndexBuffer;
	class VertexArray;
	class Texture;
	class Shader;
	class ObjMesh;

	class GLMesh: public DrawableObject
	{
	private:
		std::unique_ptr<VertexBuffer> mpVBO;
		std::unique_ptr<IndexBuffer> mpIBO;
		std::unique_ptr<VertexArray> mpVAO;
		std::vector<Texture*> mTextures;
		std::unique_ptr<ObjMesh> mpObjMesh;
		std::unordered_map<std::string, std::shared_ptr<Shader>> mShaders;
	public:
		GLMesh();
		virtual ~GLMesh();
		
		void DrawFace(const Renderer& renderer) override;
		void DrawWireFrame(const Renderer& renderer) override;

		void AddTexture(Texture* texture) { mTextures.push_back(texture); }
		void AddMesh(const std::string& filename);
		void AddShader(const std::string& shaderName, std::shared_ptr<Shader> shader);
	};
}
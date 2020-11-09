#pragma once
#include "GLOjbect.h"
#include <iostream>
#include <vector>
#include "Shader.h"

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
		Shader mShader;
	public:
		GLMesh();
		virtual ~GLMesh();
		void Draw(const Renderer& renderer, const Shader &shader) override;
		void AddTexture(Texture* texture) { mTextures.push_back(texture); }
		void AddMesh(const std::string& filename);
		void SetShader(const Shader &shader);
	};
}
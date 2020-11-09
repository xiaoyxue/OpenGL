#pragma once
#include "GLOjbect.h"
#include <iostream>
#include <vector>

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
	public:
		GLMesh();
		virtual ~GLMesh();
		void Draw(const Renderer& renderer, const Shader &shader) const override;
		//void SetVBO(VertexBuffer* vbo) { mpVBO = vbo; }
		//void SetIBO(IndexBuffer* ibo) { mpIBO = ibo; }
		//void SetVAO(VertexArray* vao) { mpVAO = vao; }
		void AddTexture(Texture* texture) { mTextures.push_back(texture); }
		void AddMesh(const std::string& filename);
		
	};
}
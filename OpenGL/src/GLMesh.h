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
	class GLMesh: public DrawableObject
	{
	private:
		VertexBuffer* mVBO;
		IndexBuffer* mIBO;
		VertexArray* mVAO;
		//Shader* mShader;
		std::vector<Texture*> mTextures;
	public:
		void Draw(const Renderer& renderer, const Shader &shader) const override;
		void SetVBO(VertexBuffer* vbo) { mVBO = vbo; }
		void SetIBO(IndexBuffer* ibo) { mIBO = ibo; }
		void SetVAO(VertexArray* vao) { mVAO = vao; }
		//void SetShader(Shader* shader) { mShader = shader; }
		void AddTexture(Texture* texture) { mTextures.push_back(texture); }
	};
}
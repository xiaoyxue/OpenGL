#pragma once
#include "GLOjbect.h"
#include "ForwardDecl.h"

namespace OpenGL
{
	class VertexArray : public GLObject
	{
	public:
		VertexArray();
		~VertexArray();

		void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
		void Bind() const;
		void UnBind() const;
	};
}
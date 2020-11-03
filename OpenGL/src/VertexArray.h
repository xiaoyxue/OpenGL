#pragma once
#include "GLOjbect.h"

namespace OpenGL
{
	class VertexBuffer;
	class VertexBufferLayout;

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
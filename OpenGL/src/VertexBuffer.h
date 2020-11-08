#pragma once
#include "GLOjbect.h"

namespace OpenGL
{
	class VertexBuffer : public GLObject
	{
	public:
		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();

		void Bind() const;
		void UnBind() const;
	};
}
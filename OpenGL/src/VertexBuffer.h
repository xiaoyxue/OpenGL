#pragma once
#include "GLOjbect.h"

namespace OpenGL
{
	class VertexBuffer : public GLObject
	{
	public:
		VertexBuffer();
		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();

		void SetData(const void* data, unsigned int size);
		void Bind() const;
		void UnBind() const;
	};
}
#include "VertexBuffer.h"
#include "OpenGL.h"

namespace OpenGL
{
	VertexBuffer::VertexBuffer(const void* data, unsigned int size)
	{
		GLCall(glGenBuffers(1, &mHandle));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, mHandle));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	}

	VertexBuffer::VertexBuffer(): GLObject()
	{

	}

	VertexBuffer::~VertexBuffer()
	{
		GLCall(glDeleteBuffers(1, &mHandle));
	}

	void VertexBuffer::SetData(const void* data, unsigned int size)
	{
		if(mHandle == -1)
			GLCall(glGenBuffers(1, &mHandle));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, mHandle));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	}

	void VertexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, mHandle));
	}

	void VertexBuffer::UnBind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
}

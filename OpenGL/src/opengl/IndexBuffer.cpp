#include "IndexBuffer.h"
#include "OpenGL.h"

namespace OpenGL
{

	IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
		: mCount(count)
	{
		ASSERT(sizeof(unsigned int) == sizeof(GLuint));
		GLCall(glGenBuffers(1, &mHandle));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHandle));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
	}

	IndexBuffer::IndexBuffer() : GLObject(), mCount(0)
	{

	}

	IndexBuffer::~IndexBuffer()
	{
		GLCall(glDeleteBuffers(1, &mHandle));
	}

	void IndexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHandle));
	}

	void IndexBuffer::UnBind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	void IndexBuffer::SetData(const unsigned int* data, unsigned int count)
	{
		if(mHandle == -1)
			GLCall(glGenBuffers(1, &mHandle));
		mCount = count;
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHandle));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
	}

}
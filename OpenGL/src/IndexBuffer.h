#pragma once
#include "GLOjbect.h"

namespace OpenGL
{
	class IndexBuffer : public GLObject
	{
	private:
		unsigned int mCount;

	public:
		IndexBuffer(const unsigned int* data, unsigned int count);
		~IndexBuffer();

		void Bind() const;
		void UnBind() const;
		inline unsigned int GetCount() const { return mCount; }
	};
}
#pragma once
#include "GLOjbect.h"

namespace OpenGL
{
	class IndexBuffer : public GLObject
	{
	private:
		unsigned int mCount;

	public:
		IndexBuffer();
		IndexBuffer(const unsigned int* data, unsigned int count);
		virtual ~IndexBuffer();

		void Bind() const;
		void UnBind() const;
		void SetData(const unsigned int* data, unsigned int count);
		inline unsigned int GetCount() const { return mCount; }
	};
}
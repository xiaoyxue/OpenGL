#pragma once

#include "OpenGL.h"
#include <vector>
#include "GLOjbect.h"

namespace OpenGL
{
	struct VertexBufferElement
	{
		unsigned int type;
		unsigned int count;
		unsigned char normalized;

		static unsigned int GetSizeOfType(unsigned int type)
		{
			switch (type)
			{
			case GL_FLOAT: return 4;
			case GL_UNSIGNED_INT: return 4;
			case GL_UNSIGNED_BYTE: return 1;
			}
			return 0;
		}
	};

	class VertexBufferLayout
	{
	private:
		std::vector<VertexBufferElement> mElements;
		unsigned int mStride;

	public:
		VertexBufferLayout() :mStride(0) {}

		template<typename T>
		void Push(unsigned int count)
		{
			static_assert(false);
		}

		template<>
		void Push<float>(unsigned int count)
		{
			mElements.push_back({ GL_FLOAT, count, GL_FALSE });
			mStride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
		}

		template<>
		void Push<unsigned int>(unsigned int count)
		{
			mElements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
			mStride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
		}

		template<>
		void Push<unsigned char>(unsigned int count)
		{
			mElements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
			mStride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
		}

		inline const std::vector<VertexBufferElement> GetElements() const { return mElements; }

		inline const unsigned int GetStride() const { return mStride; }
	};
}
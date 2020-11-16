#pragma once

#include "ForwardDecl.h"

namespace OpenGL 
{
	class GLObject
	{
	protected:
		unsigned int mHandle;
	public:
		GLObject() : mHandle(-1) {}
		GLObject(unsigned int id) : mHandle(id) {}
		virtual ~GLObject() = default;
		inline unsigned int GetHandle() const { return mHandle; }
	};

	class DrawableObject
	{
	public:
		virtual ~DrawableObject() = default;
		virtual void DrawFace(const Renderer& renderer) const {}
		virtual void DrawWireFrame(const Renderer& renderer) const {}
	};

}

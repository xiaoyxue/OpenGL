#pragma once

namespace OpenGL 
{
	class GLObject
	{
	protected:
		unsigned int mHandle;
	public:
		GLObject() : mHandle(0) {}
		GLObject(unsigned int id) : mHandle(id) {}
		inline unsigned int GetHandle() const { return mHandle; }
	};
}

#pragma once

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

}

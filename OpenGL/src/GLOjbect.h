#pragma once

namespace OpenGL 
{
	class Renderer;
	class Shader;

	class GLObject
	{
	protected:
		unsigned int mHandle;
	public:
		GLObject() : mHandle(-1) {}
		GLObject(unsigned int id) : mHandle(id) {}
		inline unsigned int GetHandle() const { return mHandle; }
	};

	class DrawableObject
	{
	public:
		virtual ~DrawableObject() = default;
		virtual void Draw(const Renderer& renderer, const Shader& shader) const = 0;
	};

}

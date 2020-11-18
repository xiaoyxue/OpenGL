#pragma once
#include "ForwardDecl.h"
#include "FrameBuffer.h"
#include "Texture.h"
#include <memory>
#include <vector>

namespace OpenGL
{
	class Picker
	{
	public:
		Picker() = default;
		Picker(int width, int height);
		void Bind();
		void UnBind();
		unsigned int ReadObjectId(int w, int d);

	private:
		int mWidth, mHeight;
		FrameBuffer mFrameBuffer;
		std::unique_ptr<Texture> mpTexture;
		std::vector<unsigned int> mIdBuffer;
	};
}

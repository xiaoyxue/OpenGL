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
		void Resize(int w, int h);
		Picker() = default;
		Picker(int width, int height);
		void Bind();
		void UnBind();
		unsigned int Pick(float x, float y, const Scene& scence, const Renderer& renderer);

	private:
		int mWidth, mHeight;
		FrameBuffer mFrameBuffer;
		RenderBuffer mRenderBuffer;
		std::unique_ptr<Texture2D> mpTexture;
	};
}

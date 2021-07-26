#include "Picker.h"
#include "Texture.h"
#include "GLImage.h"
#include "Scene.h"
#include "Renderer.h"
#include <iostream>

namespace OpenGL
{
	void Picker::Resize(int w, int h)
	{
		mWidth = w;
		mHeight = h;
		mpTexture = std::make_unique<Texture2D>(mWidth, mHeight);
		mRenderBuffer.SetStorage(mWidth, mHeight, ImageFormat::Depth32);
		mFrameBuffer.Attach(FrameBufferAttachment::Color0, mpTexture.get());
		mFrameBuffer.Attach(FrameBufferAttachment::Depth, &mRenderBuffer);
	}

	Picker::Picker(int width, int height)
	{
		mWidth = width;
		mHeight = height;
		mpTexture = std::make_unique<Texture2D>(width, height);
		mRenderBuffer.SetStorage(mWidth, mHeight, ImageFormat::Depth32);
		mFrameBuffer.Attach(FrameBufferAttachment::Color0, mpTexture.get());
		mFrameBuffer.Attach(FrameBufferAttachment::Depth, &mRenderBuffer);
	}

	void Picker::Bind()
	{
		mFrameBuffer.Bind();
	}

	void Picker::UnBind()
	{
		mFrameBuffer.UnBind();
	}

	unsigned int Picker::Pick(float x, float y, const Scene &scence, const Renderer &renderer)
	{
		renderer.Clear();
		for(auto it : scence.GetDrawObjects())
		{
			renderer.DrawObjectId(*it);
		}
		GLCall(glFlush());
		GLCall(glFinish());

		unsigned char data[4];
		GLCall(glReadPixels((int)x, mHeight - int(y) - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data));

		// Convert the color back to an integer ID
		int pickedID =
			data[0] +
			data[1] * 256 +
			data[2] * 256 * 256;

		std::cout << x << " " << y << std::endl;
		if (pickedID == 0) { // Full white, must be the background !
			std::cout << "background" << std::endl;
			return -1;
		}
		else 
		{
			std::cout << "mesh " << pickedID - 1 << std::endl;
		}
		return pickedID - 1;
	}

}


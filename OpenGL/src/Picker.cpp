#include "Picker.h"
#include "Texture.h"
#include "GLImage.h"

namespace OpenGL
{
	
	Picker::Picker(int width, int height)
	{
		mpTexture = std::make_unique<Texture>(width, height);
		mFrameBuffer.Attach(FrameBufferAttachment::Color0, mpTexture.get());
		mIdBuffer.resize(width * height);
	}

	void Picker::Bind()
	{
		mFrameBuffer.Bind();
	}

	void Picker::UnBind()
	{
		mFrameBuffer.UnBind();
	}

	unsigned int Picker::ReadObjectId(int x, int y)
	{
		mpTexture->ReadPixels(ImageFormat::RGBA_INT, ImageDataType::UnsignedInt, &mIdBuffer);
		return 0;
	}

}


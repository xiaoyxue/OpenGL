#include "Texture.h"
#include "OpenGL.h"
#include "GLImage.h"
#include "stb_image/stb_image.h"

namespace OpenGL
{
	Texture::Texture(int width, int height)
	{
		mWidth = width;
		mHeight = height;
		GLCall(glGenTextures(1, &mHandle));
		GLCall(glBindTexture(GL_TEXTURE_2D, mHandle));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}

	Texture::Texture(const std::string& path)
		:mFilePath(path), mLocalBuffer(nullptr), GLObject(0),
		mWidth(0), mHeight(0), mBPP(0)
	{
		stbi_set_flip_vertically_on_load(1);
		mLocalBuffer = stbi_load(path.c_str(), &mWidth, &mHeight, &mBPP, 4);

		GLCall(glGenTextures(1, &mHandle));
		GLCall(glBindTexture(GL_TEXTURE_2D, mHandle));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mLocalBuffer));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

		if (mLocalBuffer)
			stbi_image_free(mLocalBuffer);
	}


	Texture::~Texture()
	{
		GLCall(glDeleteTextures(1, &mHandle));
	}

	void Texture::Load(const std::string& path)
	{
		stbi_set_flip_vertically_on_load(1);
		mLocalBuffer = stbi_load(path.c_str(), &mWidth, &mHeight, &mBPP, 4);

		GLCall(glBindTexture(GL_TEXTURE_2D, mHandle));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mLocalBuffer));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		if (mLocalBuffer)
			stbi_image_free(mLocalBuffer);
	}

	void Texture::Bind(unsigned int slot /*= 0*/) const
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, mHandle));
	}

	void Texture::UnBind() const
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}

	void Texture::ReadPixels(ImageFormat outputFormat, ImageDataType type, void* data)
	{
		glBindTexture(GL_TEXTURE_2D, mHandle);
		glGetTexImage(GL_TEXTURE_2D, 0, (int)outputFormat, (int)type, data);
	}

}
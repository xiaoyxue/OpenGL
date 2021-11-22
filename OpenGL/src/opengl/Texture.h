#pragma once
//#define STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "OpenGL.h"
#include "GLOjbect.h"
#include "ForwardDecl.h"
#include "GLImage.h"
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"
#include <string>

namespace OpenGL
{
	enum class TextureFilter
	{
		Nearest = 0,
		Linear = 1,
		TriLinear = 2,
		Anisotropic4x = 3,
		Anisotropic8x = 4,
		Anisotropic16x = 5
	};

	enum class TextureWrapMode
	{
		Clamp, Repeat, Mirror
	};


	enum class TextureMode {
		GL_Texture2D = GL_TEXTURE_2D
	};

	template<int Target>
	class Texture : public GLObject
	{
	public:
		Texture()
		{
			GLCall(glGenTextures(1, &mHandle));
			GLCall(Bind());
			GLCall(glTexParameteri(Target, GL_TEXTURE_WRAP_S, GL_REPEAT));
			GLCall(glTexParameteri(Target, GL_TEXTURE_WRAP_T, GL_REPEAT));
			GLCall(glTexParameteri(Target, GL_TEXTURE_WRAP_R, GL_REPEAT));
			//SetFilter(TextureFilter::Anisotropic16x);
			GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		}

		Texture(int w, int h, ImageFormat imageFormat, ImageDataType dataFromat, ImageFormat internalFormat = ImageFormat::RGBA, void* pLocalBuffer = nullptr)
		{
			GLCall(glGenTextures(1, &mHandle));
			GLCall(Bind());
			//GLCall(glTexParameteri(Target, GL_TEXTURE_WRAP_S, GL_REPEAT));
			//GLCall(glTexParameteri(Target, GL_TEXTURE_WRAP_T, GL_REPEAT));
			//GLCall(glTexParameteri(Target, GL_TEXTURE_WRAP_R, GL_REPEAT));
			//GLCall(glTexParameteri(Target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			//GLCall(glTexParameteri(Target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));


			GLCall(glTexParameteri(Target, GL_TEXTURE_WRAP_S, GL_REPEAT));
			GLCall(glTexParameteri(Target, GL_TEXTURE_WRAP_T, GL_REPEAT));
			GLCall(glTexParameteri(Target, GL_TEXTURE_WRAP_R, GL_REPEAT));

			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, int(internalFormat), w, h, 0, int(imageFormat), int(dataFromat), pLocalBuffer));
			//SetFilter(TextureFilter::Anisotropic16x);
			GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		}

		~Texture()
		{
			GLCall(glDeleteTextures(1, &mHandle));
		}

		void Bind(unsigned int slot = 0) const
		{
			GLCall(glActiveTexture(GL_TEXTURE0 + slot));
			GLCall(glBindTexture(Target, mHandle));
		}

		void UnBind() const
		{
			GLCall(glBindTexture(Target, 0));
		}

		void ReadPixels(ImageFormat outputFormat, ImageDataType type, void* data)
		{
			glBindTexture(Target, mHandle);
			glGetTexImage(Target, 0, (int)outputFormat, (int)type, data);
		}


		void SetFilter(TextureFilter filter)
		{
			Bind();
			switch (filter)
			{
			case TextureFilter::Nearest:
				glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				break;
			case TextureFilter::Linear:
				glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				break;
			case TextureFilter::TriLinear:
				glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				break;
			case TextureFilter::Anisotropic4x:
				glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameterf(Target, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f);
				break;
			case TextureFilter::Anisotropic8x:
				glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameterf(Target, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f);
				break;
			case TextureFilter::Anisotropic16x:
				glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameterf(Target, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
				break;
			}
		}
	};

	class Texture2D : public Texture<GL_TEXTURE_2D> {
	protected:
		unsigned char* mLocalBuffer;
		int mWidth, mHeight, mBPP;
	public:
		Texture2D() = default;

		Texture2D(int width, int height, ImageFormat imageFormat, ImageDataType dataType, ImageFormat internalFormat = ImageFormat::RGBA, void* pLocalBuffer = nullptr)
			: mWidth(width), mHeight(height), mBPP(0), mLocalBuffer((unsigned char*)pLocalBuffer), Texture(width, height, imageFormat, dataType, internalFormat, pLocalBuffer) {}

		Texture2D(int width, int height, int BPP = 0, ImageDataType dataFormat = ImageDataType::Byte)
			: mWidth(width), mHeight(height), mBPP(0), mLocalBuffer(nullptr)
		{
			mWidth = width;
			mHeight = height;
			GLCall(glGenTextures(1, &mHandle));
			GLCall(glBindTexture(GL_TEXTURE_2D, mHandle));

			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

			//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT));

			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, int(dataFormat), NULL));
			GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		}
		Texture2D(const std::string& path, ImageDataType dataFormat = ImageDataType::Byte) {
			//glGenTextures(1, &mHandle);
			//Bind();
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
			//SetFilter(TextureFilter::Anisotropic16x);
			//Load(path);

			stbi_set_flip_vertically_on_load(1);
			mLocalBuffer = stbi_load(path.c_str(), &mWidth, &mHeight, &mBPP, 4);
			GLCall(glGenTextures(1, &mHandle));
			GLCall(glBindTexture(GL_TEXTURE_2D, mHandle));


			Load(ImageFormat::RGBA, ImageFormat::RGBA, ImageDataType::Byte, mLocalBuffer, mWidth, mHeight);
			SetFilter(TextureFilter::Linear);
			//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT));

			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, int(dataFormat), mLocalBuffer));
			GLCall(glBindTexture(GL_TEXTURE_2D, 0));
			if (mLocalBuffer)
				stbi_image_free(mLocalBuffer);
		}

		void Load(ImageFormat internalFormat, ImageFormat inputFormat, ImageDataType inputType, void* data, int width, int height)
		{
			glBindTexture(GL_TEXTURE_2D, mHandle);
			glTexImage2D(GL_TEXTURE_2D, 0, (int)inputFormat, width, height, 0, (int)internalFormat, (int)inputType, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		inline int GetWidth() const { 
			return mWidth; 
		}

		inline int GetHeight() const { 
			return mHeight; 
		}

	};



}
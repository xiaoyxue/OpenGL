#pragma once

#include "GLOjbect.h"
#include "ForwardDecl.h"
#include <string>

namespace OpenGL
{

	class Texture : public GLObject
	{
	protected:
		unsigned char* mLocalBuffer;
		int mWidth, mHeight, mBPP;
	public:
		Texture() = default;
		Texture(int width, int height, int BPP = 0): mWidth(width), mHeight(height), mBPP(0), mLocalBuffer(nullptr){}
		virtual ~Texture();
		virtual void Load(const std::string& path);
		virtual void Bind(unsigned int slot = 0) const;
		virtual void UnBind() const;
		virtual void ReadPixels(ImageFormat outputFormat, ImageDataType type, void* data);
		virtual inline int GetWidth() const { return mWidth; };
		virtual inline int GetHeight() const { return mHeight; };
	};




	class GLTexture : public Texture
	{
	protected:
		std::string mFilePath;
		//unsigned char* mLocalBuffer;
		//int mWidth, mHeight, mBPP;
	public:
		GLTexture() = default;
		GLTexture(int width, int height);
		GLTexture(const std::string& path);
		~GLTexture() = default;
	};
}
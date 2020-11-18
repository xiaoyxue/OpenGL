#pragma once

#include "GLOjbect.h"
#include "ForwardDecl.h"
#include <string>

namespace OpenGL
{
	class Texture : public GLObject
	{
	private:
		std::string mFilePath;
		unsigned char* mLocalBuffer;
		int mWidth, mHeight, mBPP;

	public:
		Texture(int width, int height);
		Texture(const std::string& path);
		~Texture();

		void Load(const std::string& path);
		void Bind(unsigned int slot = 0) const;
		void UnBind() const;
		void ReadPixels(ImageFormat outputFormat, ImageDataType type, void* data);
		inline int GetWidth() const { return mWidth; }
		inline int GetHeight() const { return mHeight; }
	};
}
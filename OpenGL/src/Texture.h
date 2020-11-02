#pragma once

#include "Renderer.h"	
#include "GLOjbect.h"
#include <string>

class Texture: public GLObject
{
private:
	std::string mFilePath;
	unsigned char* mLocalBuffer;
	int mWidth, mHeight, mBPP;

public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void UnBind() const;

	inline int GetWidth() const { return mWidth; }
	inline int GetHeight() const { return mHeight; }
};
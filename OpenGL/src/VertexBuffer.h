#pragma once
#include "GLOjbect.h"

class VertexBuffer: public GLObject
{
private:
	unsigned int mHandle;

public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void UnBind() const;
};
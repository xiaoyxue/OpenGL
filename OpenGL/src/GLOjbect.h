#pragma once

class GLObject 
{
protected:
	unsigned int mHandle;
public:
	GLObject(){}
	GLObject(unsigned int id) : mHandle(id){}
	inline unsigned int GetHandle() const { return mHandle; }
};
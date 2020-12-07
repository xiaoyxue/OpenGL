#pragma once
#include <string>
#include "ForwardDecl.h"


class Window
{
protected:
	int mWidth, mHeight;
	std::string mTitle;
public:
	Window() = default;
	Window(const std::string& title, int w = 1024, int h = 768);
	virtual ~Window() = default;

	virtual void SetSize(int w, int h);
	virtual void MainLoop() {}

};


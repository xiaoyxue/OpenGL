#include "Window.h"
#include <iostream>


Window::Window(const std::string& title, int w /*= 1024*/, int h /*= 768*/)
	: mTitle(title)
{
	mWidth = w;
	mHeight = h;
}

void Window::SetSize(int w, int h)
{
	mWidth = w;
	mHeight = h;
}



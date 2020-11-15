#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "../ForwardDecl.h"

namespace GLFW
{

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

}

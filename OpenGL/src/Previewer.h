#pragma once
#include "ForwardDecl.h"
#include "Window.h"
#include <vector>


namespace GLFW
{
	using namespace OpenGL;
	class Previewer : public GLFWWindow
	{
	private:
		std::vector<DrawableObject*> mDrawableObjects;
	public:
		Previewer() = default;
		Previewer(const std::string& title, int w = 1024, int h = 760);
		virtual ~Previewer();

		void DrawAll() const override;

		void AddDrawableObject(DrawableObject* pObject);

	};
}


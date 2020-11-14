#pragma once
#include "ForwardDecl.h"
#include "window/GLWindow.h"
#include <vector>


namespace GLFW
{
	using namespace OpenGL;

	class Previewer : public GLWindow
	{
	private:
		std::vector<DrawableObject*> mDrawableObjects;
	public:
		Previewer() = default;
		Previewer(const std::string& title, int w = 1024, int h = 768);
		virtual ~Previewer();

		void Init();
		void DrawAll() const override;
		void AddDrawableObject(DrawableObject* pObject);

	protected:
		bool HandleMouseEvent() override;
	};
}


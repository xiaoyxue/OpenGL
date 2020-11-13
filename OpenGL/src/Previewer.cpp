#include "Previewer.h"
#include "Renderer.h"


namespace GLFW
{
	Previewer::Previewer(const std::string& title, int w /*= 1024*/, int h /*= 760*/)
		:GLFWWindow(title, w, h)
	{

	}

	Previewer::~Previewer()
	{

	}

	void Previewer::DrawAll() const
	{
		mpRenderer->Clear();
		for (auto it : mDrawableObjects)
		{
			mpRenderer->DrawFaces(*it);
			mpRenderer->DrawWireFrame(*it);
		}

	}

	void Previewer::AddDrawableObject(DrawableObject* pObject)
	{
		mDrawableObjects.push_back(pObject);
	}

}


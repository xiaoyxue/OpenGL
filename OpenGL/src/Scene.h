#pragma once
#include "ForwardDecl.h"
#include <vector>

namespace OpenGL 
{
	class Scene
	{
	private:
		std::vector<DrawableObject*> mDrawableObjects;
	public:
		const std::vector<DrawableObject*>& GetDrawObjects() const 
		{ 
			return mDrawableObjects; 
		}
		void AddDrawableObject(DrawableObject* pObject) 
		{ 
			mDrawableObjects.push_back(pObject); 
		}
	};

}

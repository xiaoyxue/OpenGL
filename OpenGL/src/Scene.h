#pragma once
#include "ForwardDecl.h"
#include "math/AABB.h"
#include <vector>

namespace OpenGL 
{
	using namespace Math;
	class Scene
	{
	private:
		std::vector<DrawableObject*> mDrawableObjects;
		BBox mBBox;
	public:
		inline std::vector<DrawableObject*>& GetDrawObjects() 
		{ 
			return mDrawableObjects; 
		}
		void AddDrawableObject(DrawableObject* pObject);
	};

}

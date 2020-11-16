#include "Scene.h"
#include "GLOjbect.h"

namespace OpenGL
{

	void Scene::AddDrawableObject(DrawableObject* pObject)
	{
		mDrawableObjects.push_back(pObject);
		mBBox = Union(mBBox, pObject->GetBBox());
	}

}


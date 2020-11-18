#include "Scene.h"
#include "DrawableObject.h"

namespace OpenGL
{

	void Scene::AddDrawableObject(DrawableObject* pObject)
	{
		mDrawableObjects.push_back(pObject);
		pObject->SetObjectId(mObjectCount++);
		mBBox = Union(mBBox, pObject->GetBBox());
	}

}


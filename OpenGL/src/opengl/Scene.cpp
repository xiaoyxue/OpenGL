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

	void Scene::TranslateSelection(float dx, float dy, float dz)
	{
		if (mPickId >= 0)
			mDrawableObjects[mPickId]->Translate(dx, dy, dz);
	}

	void Scene::RotateSelection(const std::string &mode, float dPhi, float dTheta)
	{
		if (mPickId >= 0)
		{
			if ((mode == "X") || (mode == "Y") || (mode == "XY"))
				mDrawableObjects[mPickId]->Rotate(dPhi, dTheta);
			else if (mode == "Z")
				mDrawableObjects[mPickId]->RotateAroundZ(dPhi);
		}
	}

	void Scene::ScaleSelection(float scale)
	{
		if (mPickId >= 0)
			mDrawableObjects[mPickId]->Scale(scale);
	}

	void Scene::TranslateSelectionLocal(float dx, float dy, float dz)
	{
		if (mPickId >= 0)
			mDrawableObjects[mPickId]->TranslateLocal(dx, dy, dz);
	}

	void Scene::RotateSelectionLocal(const std::string &mode, float dPhi, float dTheta)
	{
		if (mPickId >= 0)
		{
			if((mode == "X") || (mode == "Y") || (mode == "XY"))
				mDrawableObjects[mPickId]->RotateLocal(dPhi, dTheta);
			else if(mode == "Z")
				mDrawableObjects[mPickId]->RotateAroundZLocal(dPhi);
		}
			
	}

	void Scene::ScaleSelectionLocal(float scale)
	{
		if (mPickId >= 0)
			mDrawableObjects[mPickId]->ScaleLocal(scale);
	}

}


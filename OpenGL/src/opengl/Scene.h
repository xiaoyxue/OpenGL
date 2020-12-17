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
		std::vector<DrawableObject*> mBackgrounds;
		std::vector<DrawableObject*> mDrawableObjects;
		BBox mBBox;
		unsigned int mObjectCount;
		int mPickId;
	public:
		Scene() : mObjectCount(0), mPickId(-1) {}
		inline const std::vector<DrawableObject*>& GetDrawObjects() const { return mDrawableObjects; }
		inline void SetPickId(int id) { mPickId = id; }
		inline DrawableObject* PickObject() const { return mDrawableObjects[mPickId]; }
		virtual inline BBox GetBBox() const { return mBBox; }
		void AddDrawableObject(DrawableObject* pObject);
		void TranslateSelection(float dx, float dy, float dz);
		void RotateSelection(const std::string& mode, float dPhi, float dTheta);
		void ScaleSelection(float scale);
		void TranslateSelectionLocal(float dx, float dy, float dz);
		void RotateSelectionLocal(const std::string& mode, float dPhi, float dTheta);
		void ScaleSelectionLocal(float scale);
		inline void ResetPick() { mPickId = -1; }
		void AddBackground(DrawableObject* pObject) { mBackgrounds.push_back(pObject); }
		inline std::vector<DrawableObject*>& GetBackground() { return mBackgrounds; }
		bool IsSelected() const { return mPickId != -1; }
	};

}

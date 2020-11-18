#pragma once
#include "ForwardDecl.h"
#include "math/AABB.h"
#include "math/Transform.h"
#include "Shader.h"

namespace OpenGL
{
	using namespace Math;
	class DrawableObject
	{
	public:
		DrawableObject();
		virtual ~DrawableObject() = default;
		virtual void DrawFace(const Renderer& renderer) const {}
		virtual void DrawWireFrame(const Renderer& renderer) const {}
		virtual void DrawObjectId(const Renderer& renderer) const {}

		virtual void Rotate(float dPhi, float dTheta);
		virtual void Translate(float dx, float dy, float dz);
		virtual void Scale(float scale);
		virtual inline unsigned int GetObjectId() const { return mObjectId; }
		virtual inline void SetObjectId(unsigned int id) { mObjectId = id; }
		virtual BBox GetBBox() const = 0;
	public:
		Transform WorldToLocal, LocalToWorld;
	protected:
		unsigned int mObjectId;
		mutable Shader mSelectorShader;
	};
}
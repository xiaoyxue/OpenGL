#pragma once

#include "ForwardDecl.h"
#include "math/AABB.h"
#include "math/Transform.h"

namespace OpenGL 
{
	using namespace Math;
	class GLObject
	{
	protected:
		unsigned int mHandle;
	public:
		GLObject() : mHandle(-1) {}
		GLObject(unsigned int id) : mHandle(id) {}
		virtual ~GLObject() = default;
		inline unsigned int GetHandle() const { return mHandle; }
	};

	class DrawableObject
	{
	public:
		virtual ~DrawableObject() = default;
		virtual void DrawFace(const Renderer& renderer) const {}
		virtual void DrawWireFrame(const Renderer& renderer) const {}
		virtual BBox GetBBox() const = 0;

		virtual void Rotate(float dPhi, float dTheta);
		virtual void Translate(float dx, float dy, float dz);
		virtual void Scale(float scale);
		virtual inline unsigned int GetObjectId() const { return mObjectId; }
		virtual inline void SetObjectId(unsigned int id) { mObjectId = id; }
	public:
		Transform WorldToLocal, LocalToWorld;
	private:
		unsigned int mObjectId;
	};

}

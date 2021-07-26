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
		virtual void DrawToGbuffer(const Renderer& renderer, const FrameBuffer &frameBuffer) const {}
		virtual void DebugDraw(const Renderer& renderer) const {}
		virtual void DrawSSAO(const Renderer& renderer, const SSAO& ssaoBuffer) const {}
		virtual void DrawToFrameBuffer(const Renderer& renderer, const FrameBuffer& frameBuffer) const {}

		virtual void Rotate(float dPhi, float dTheta);
		virtual void Translate(float dx, float dy, float dz);
		virtual void Scale(float scale);
		virtual void RotateLocal(float dPhi, float dTheta);
		virtual void RotateAroundZ(float dTheta);
		virtual void RotateAroundZLocal(float dTheta);
		virtual void ScaleLocal(float scale);
		virtual void TranslateLocal(float dx, float dy, float dz);
		virtual inline unsigned int GetObjectId() const { return mObjectId; }
		virtual inline void SetObjectId(unsigned int id) { mObjectId = id; }
		virtual BBox GetBBox() const = 0;
		virtual bool& GetTraceFlag() { return mTraceFlag; }
		virtual const bool& GetTraceFlag() const { return mTraceFlag; }
	public:
		Transform WorldToLocal, LocalToWorld;
		Matrix4 mTraceMatrix;
	protected:
		unsigned int mObjectId;
		mutable Shader mSelectorShader;
		mutable Shader mDebugShader;
	private:
		bool mTraceFlag = false;
	};
}
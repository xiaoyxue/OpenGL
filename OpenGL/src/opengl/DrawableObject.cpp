#include "DrawableObject.h"
#include "math/Transform.h"
#include "math/Lingal.h"
#include "Renderer.h"
#include "visual/Camera.h"

namespace OpenGL
{
	using namespace Math;

	void DrawableObject::Rotate(float dPhi, float dTheta)
	{
		LocalToWorld = Transform::RotateX(dTheta) * Transform::RotateY(dPhi) * LocalToWorld;
		WorldToLocal = Inverse(LocalToWorld);
	}

	void DrawableObject::Translate(float dx, float dy, float dz)
	{
		LocalToWorld = Transform::Translate(Vec3(dx, dy, dz)) * LocalToWorld;
		WorldToLocal = Inverse(LocalToWorld);
	}

	void DrawableObject::Scale(float scale)
	{
		LocalToWorld = Transform::Scale(scale, scale, scale) * LocalToWorld;
		WorldToLocal = Inverse(LocalToWorld);
	}


	void DrawableObject::RotateLocal(float dPhi, float dTheta)
	{
		Transform moveToWorldCenter = Transform::Translate(-1 * GetBBox().Center());
		WorldToLocal = WorldToLocal * (Inverse(moveToWorldCenter) * Transform::RotateY(dPhi)* Transform::RotateX(dTheta) * moveToWorldCenter);
		LocalToWorld = Inverse(WorldToLocal);

	}

	void DrawableObject::RotateAroundZ(float dTheta)
	{
		LocalToWorld = Transform::RotateZ(dTheta) * LocalToWorld;
		WorldToLocal = Inverse(LocalToWorld);
	}
	void DrawableObject::RotateAroundZLocal(float dTheta)
	{
		Transform moveToWorldCenter = Transform::Translate(-1 * GetBBox().Center());
		WorldToLocal = WorldToLocal * (Inverse(moveToWorldCenter) * Transform::RotateZ(dTheta) * moveToWorldCenter);
		LocalToWorld = Inverse(WorldToLocal);
	}

	void DrawableObject::ScaleLocal(float scale)
	{
		LocalToWorld =  LocalToWorld * Transform::Scale(scale, scale, scale);
		WorldToLocal = Inverse(LocalToWorld);
	}


	void DrawableObject::TranslateLocal(float dx, float dy, float dz)
	{
		LocalToWorld = LocalToWorld * Transform::Translate(Vec3(dx, dy, dz));
		WorldToLocal = Inverse(LocalToWorld);
	}


	DrawableObject::DrawableObject()
		: mObjectId(-1), mSelectorShader("res/shaders/ObjectPicker.shader")
	{
		mTraceMatrix = Matrix4(1);
	}

}
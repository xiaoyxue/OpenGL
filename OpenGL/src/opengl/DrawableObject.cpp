#include "DrawableObject.h"
#include "math/Transform.h"
#include "math/Lingal.h"
#include "Renderer.h"

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
		LocalToWorld =  LocalToWorld * Transform::RotateX(dTheta) * Transform::RotateY(dPhi);
		WorldToLocal = Inverse(LocalToWorld);
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

	}

}
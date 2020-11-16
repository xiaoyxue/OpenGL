#include "GLOjbect.h"
#include "math/Transform.h"
#include "math/Lingal.h"

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

}


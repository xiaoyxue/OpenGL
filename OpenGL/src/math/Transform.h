#pragma once
#include "Utils.h"
#include "Lingal.h"
#include "AABB.h"

namespace Math
{
	class Transform {
	public:
		Transform() {
			mat = Matrix4::Identity();
			invMat = Matrix4::Identity();
		}

		Transform(
			real m00, real m01, real m02, real m03,
			real m10, real m11, real m12, real m13,
			real m20, real m21, real m22, real m23,
			real m30, real m31, real m32, real m33)
		{
			mat = Matrix4(
				m00, m01, m02, m03,
				m10, m11, m12, m13,
				m20, m21, m22, m23,
				m30, m31, m32, m33);

			invMat = Inverse(mat);
		}

		Transform(const real m[4][4]) {
			mat = Matrix4(
				m[0][0], m[0][1], m[0][2], m[0][3],
				m[1][0], m[1][1], m[1][2], m[1][3],
				m[2][0], m[2][1], m[2][2], m[2][3],
				m[3][0], m[3][1], m[3][2], m[3][3]);

			invMat = Inverse(mat);
		}

		Transform(const Matrix4& m) {
			mat = m;
			invMat = Inverse(m);
		}

		Transform(const Matrix4& m, const Matrix4& invM) {
			mat = m;
			invMat = invM;
		}

		Transform(const Transform& t) {
			mat = t.mat;
			invMat = t.invMat;
		}

		friend Transform Inverse(const Transform& t) {
			return Transform(t.invMat, t.mat);
		}

		friend  Transform Transpose(const Transform& t) {
			return Transform(t.mat.Transpose(), t.invMat.Transpose());
		}

		FORCE_INLINE const Matrix4& GetMatrix() const {
			return mat;
		}

		FORCE_INLINE const Matrix4& GetInverseMatrix() const {
			return invMat;
		}

		FORCE_INLINE Transform operator*(const Transform& t) const {
			return Transform(mat * t.mat, t.invMat * invMat);
		}

		FORCE_INLINE Vector3 operator()(const Vector3& p) const {
			Vector4 pp(p.x, p.y, p.z, 1.0);
			pp = mat * pp;
			if (pp.w == 1 || pp.w == 0) {
				return Vector3(pp.x, pp.y, pp.z);
			}
			else {
				return Vector3(pp.x, pp.y, pp.z) / pp.w;
			}
		}

		FORCE_INLINE AABB operator()(const AABB& bound) const{
			const Transform& transform = *this;
			AABB ret;
			ret = Union(ret, transform(Vec3(bound.minPoint.x, bound.minPoint.y, bound.minPoint.z)));
			ret = Union(ret, transform(Vec3(bound.maxPoint.x, bound.minPoint.y, bound.minPoint.z)));
			ret = Union(ret, transform(Vec3(bound.minPoint.x, bound.maxPoint.y, bound.minPoint.z)));
			ret = Union(ret, transform(Vec3(bound.minPoint.x, bound.minPoint.y, bound.maxPoint.z)));
			ret = Union(ret, transform(Vec3(bound.minPoint.x, bound.maxPoint.y, bound.maxPoint.z)));
			ret = Union(ret, transform(Vec3(bound.maxPoint.x, bound.maxPoint.y, bound.minPoint.z)));
			ret = Union(ret, transform(Vec3(bound.maxPoint.x, bound.minPoint.y, bound.maxPoint.z)));
			ret = Union(ret, transform(Vec3(bound.maxPoint.x, bound.maxPoint.y, bound.maxPoint.z)));
			return ret;
		}

		static Transform Translate(const Vector3& v);
		static Transform Scale(real sx, real sy, real sz);
		static Transform RotateX(real theta);
		static Transform RotateY(real theta);
		static Transform RotateZ(real theta);
		static Transform Rotate(real theta, const Vector3& axis);
		static Transform LookAt(const Vector3& pos, const Vector3& look, const Vector3& up);
		static Transform Orthographic(real znear, real zfar);
		static Transform Orthographic(real fovy, real aspect, real dis, real znear, real zfar);
		static Transform Perspective(real fovy, real aspect, real dis, real znear, real zfar);

		
	private:
		Matrix4 mat, invMat;
	};
}
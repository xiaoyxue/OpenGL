#pragma once
#include "Def.h"
#include "math/Lingal.h"
#include "math/Transform.h"
#include "Utils.h"
#include <iostream>
#include <algorithm>

namespace OpenGL
{
	enum class CameraType
	{
		Perspective = 0,
		Orthographic = 1
	};
	using namespace Math;
	class Camera 
	{
	private:
		float mFovy;
		float mDistanceToFilm;
		float mNear, mFar;
		int mWidth, mHeight;
		float mR, mTheta, mPhi;
		Vec3 mPosition, mLookAt, mUp;
		Vec3 mCx, mCy, mCz;
		Matrix4 mViewMatrix;
		Matrix4 mProjMatrix;

		float mMoveSpeed, mScaleFactor;
		float mMaxR, mMinR;
		CameraType mCameraType;
	public:
		void Init(
			const Vec3& position,
			const Vec3& lookat,
			const Vec3& up,
			int width,
			int height,
			CameraType cameraType = CameraType::Perspective,
			float fovy = 45.f,
			float disToFilm = 1.0f,
			float nClip = 0.1f,
			float fClip = 1000.0f
		)
		{
			mCameraType = cameraType;
			mPosition = position;
			mLookAt = lookat;
			mWidth = width;
			mHeight = height;
			mFovy = fovy;
			mDistanceToFilm = disToFilm;
			mNear = nClip;
			mFar = fClip;
			mUp = up;

			mMoveSpeed = 2.5f;
			mScaleFactor = 2.0f;
			mMinR = 0.001f;
			mMaxR = 10000000.f;

			ComputeSphereCoord();
			ComputePosition();
			ComputeViewMatrix();
			ComputeProjMatrix();
		}

		void Place(const Vec3& position)
		{
			mPosition = position;
			ComputeViewMatrix();
		}

		void Resize(int width, int height)
		{
			mWidth = width;
			mHeight = height;
			ComputeProjMatrix();
		}

		void Scale(float d)
		{
			mR = std::min(mMaxR, std::max(mMinR, mR + d * mScaleFactor));
			ComputePosition();
			ComputeViewMatrix();
		}

		void Move(float dx, float dy)
		{
			Vec3 moveDir = dx * mMoveSpeed * mCx + dy * mMoveSpeed * mCy;
			mPosition += moveDir;
			mLookAt += moveDir;
			ComputeViewMatrix();
		}

		void MoveX(float dx)
		{
			Vec3 moveDir = dx * mMoveSpeed * mCx;
			mPosition += moveDir;
			mLookAt += moveDir;
			ComputeViewMatrix();
		}

		void MoveY(float dy)
		{
			Vec3 moveDir = dy * mMoveSpeed * mCy;
			mPosition += moveDir;
			mLookAt += moveDir;
			ComputeViewMatrix();
		}

		void Rotate(float dPhi, float dTheta) 
		{
			mPhi += dPhi;
			mTheta = Clamp(mTheta + dTheta, 0.f, PI - EPS_F);
			ComputePosition();
			ComputeViewMatrix();
		}

		inline Matrix4 GetViewMatrix() const { return mViewMatrix; }

		inline Matrix4 GetProjMatrix() const { return mProjMatrix; }


	private:
		
		void ComputePosition()
		{
			float sinTheta = std::sin(mTheta);
			if (sinTheta == 0)
			{
				mTheta += EPS_F;
				sinTheta = std::sin(mTheta);
			}
			Vec3 dirToCam(
				mR * sinTheta * std::cos(mPhi),
				mR * std::cos(mTheta),
				mR * sinTheta * std::sin(mPhi));
			mPosition = mLookAt + dirToCam;

			mCz = (mPosition - mLookAt).Norm(); // -1 * dir in raytracer
			mCx = Cross(mUp, mCz).Norm();
			mCy = Cross(mCz, mCx);
		}

		void ComputeSphereCoord()
		{
			mR = float((mPosition - mLookAt).Length());
			mPhi = std::atan2(mPosition.z, mPosition.x);
			mPhi = mPhi > 0 ? mPhi : 2.f * PI - mPhi;
			mTheta = std::acos(mPosition.y / mR);
		}

		void ComputeViewMatrix()
		{
			mViewMatrix = Transform::LookAt(mPosition, mLookAt, mUp).GetMatrix();
		}

		void ComputeProjMatrix()
		{
			if (mCameraType == CameraType::Perspective)
				mProjMatrix = Transform::Perspective(mFovy, (float)mWidth / (float)mHeight, mDistanceToFilm, mNear, mFar).GetMatrix();
			else if (mCameraType == CameraType::Orthographic)
				mProjMatrix = Transform::Orthographic(mFovy, (float)mWidth / (float)mHeight, mDistanceToFilm, mNear, mFar).GetMatrix();
		}
	};
}
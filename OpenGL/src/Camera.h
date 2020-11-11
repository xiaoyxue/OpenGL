#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"
#include "Def.h"
#include <iostream>
#include <algorithm>

namespace OpenGL
{
	class Camera 
	{
		using vec3 = glm::vec3;
		using mat4 = glm::mat4;
	private:
		float mFovy;
		float mFocusDistance;
		float mNear, mFar;
		int mWidth, mHeight;
		float mR, mTheta, mPhi;
		vec3 mPosition, mLookAt, mUp;
		vec3 mCx, mCy, mCz;
		mat4 mViewMatrix;
		mat4 mProjMatrix;

		float mMoveSpeed, mScaleFactor;
		float mMaxR, mMinR;
	public:
		void Init(
			const vec3& position,
			const vec3& lookat,
			const vec3& up,
			int width,
			int height,
			float fovy = 45.f,
			float focusDistance = 1.0f,
			float nClip = 0.01f,
			float fClip = 1000.0f
		)
		{
			mPosition = position;
			mLookAt = lookat;
			mWidth = width;
			mHeight = height;
			mFovy = fovy;
			mFocusDistance = focusDistance;
			mNear = nClip;
			mFar = fClip;
			mUp = up;

			mMoveSpeed = 2.5f;
			mScaleFactor = 2.0f;
			mMinR = 0.001f;
			mMaxR = 100000.f;

			ComputeSphereCoord();
			ComputePosition();
			ComputeViewMatrix();
			ComputeProjMatrix();
		}

		void Place(const vec3& position)
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
			vec3 moveDir = dx * mMoveSpeed * mCx + dy * mMoveSpeed * mCy;
			mPosition += moveDir;
			mLookAt += moveDir;
			ComputeViewMatrix();
		}

		void MoveX(float dx)
		{
			vec3 moveDir = dx * mMoveSpeed * mCx;
			mPosition += moveDir;
			mLookAt += moveDir;
			ComputeViewMatrix();
		}

		void MoveY(float dy)
		{
			vec3 moveDir = dy * mMoveSpeed * mCy;
			mPosition += moveDir;
			mLookAt += moveDir;
			ComputeViewMatrix();
		}

		void Rotate(float dPhi, float dTheta) 
		{
			mPhi += dPhi;
			mTheta = glm::clamp(mTheta + dTheta, 0.f, PI - EPS_F);
			ComputePosition();
			ComputeViewMatrix();
		}

		inline mat4 GetViewMatrix() const { return mViewMatrix; }

		inline mat4 GetProjMatrix() const { return mProjMatrix; }

	private:
		
		void ComputePosition()
		{
			float sinTheta = std::sin(mTheta);
			if (sinTheta == 0)
			{
				mTheta += EPS_F;
				sinTheta = std::sin(mTheta);
			}
			vec3 dirToCam(
				mR * sinTheta * std::cos(mPhi),
				mR * std::cos(mTheta),
				mR * sinTheta * std::sin(mPhi));
			mPosition = mLookAt + dirToCam;
			mCz = glm::normalize(mPosition - mLookAt); // -1 * dir in raytracer
			mCx = glm::normalize(glm::cross(mUp, mCz));
			mCy = glm::cross(mCz, mCx);
		}

		void ComputeSphereCoord()
		{
			mR = float((mPosition - mLookAt).length());
			mPhi = std::atan2(mPosition.z, mPosition.x);
			mPhi = mPhi > 0 ? mPhi : 2.f * PI - mPhi;
			mTheta = std::acos(mPosition.y / mR);
		}

		void ComputeViewMatrix()
		{
			mViewMatrix = glm::lookAt(mPosition, mLookAt, mUp);
		}

		void ComputeProjMatrix()
		{
			mProjMatrix = glm::perspective(mFovy, (float)mWidth / (float)mHeight, mNear, mFar);
		}
	};
}
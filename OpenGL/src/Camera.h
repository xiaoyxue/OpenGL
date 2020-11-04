#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"
#include "Def.h"
#include <iostream>

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

		float mMoveSpeed;
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

			mCz = glm::normalize(mPosition - mLookAt); // -1 * dir in raytracer
			mCx = glm::normalize(glm::cross(mUp, mCz));
			mCy = glm::cross(mCz, mCx);
			
			ComputeSphereCoord();

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

		void MoveX(float scale)
		{
			mPosition = mPosition + scale * mMoveSpeed * mCx;
			ComputeSphereCoord();
			ComputeViewMatrix();
		}

		void MoveY(float scale)
		{
			mPosition = mPosition + scale * mMoveSpeed * mCy;
			ComputeSphereCoord();
			ComputeViewMatrix();
		}

		void Rotate(float dPhi, float dTheta) 
		{
			mPhi += dPhi;
			mTheta = glm::clamp(mTheta + dTheta, 0.f, PI);
			float sinTheta = std::sin(mTheta);
			float x = mR * sinTheta * std::cos(mPhi);
			float y = mR * std::cos(mTheta);
			float z = mR * sinTheta * std::sin(mPhi);
			mPosition = vec3(x, y, z);
			ComputeDirction();
		}

		inline mat4 GetViewMatrix() const { return mViewMatrix; }

		inline mat4 GetProjMatrix() const { return mProjMatrix; }

	private:
		void ComputeViewMatrix()
		{
			float sinTheta = std::sin(mTheta);
			float x = mR * sinTheta * std::cos(mPhi);
			float y = mR * std::cos(mTheta);
			float z = mR * sinTheta * std::sin(mPhi);
			mPosition = vec3(x, y, z);
			mViewMatrix = glm::lookAt(mPosition, mPosition + (-1.f) * mCz, mUp);
		}

		void ComputeSphereCoord()
		{
			mR = (mPosition - mLookAt).length();
			mPhi = std::atan2(mPosition.z, mPosition.x);
			mPhi = mPhi > 0 ? mPhi : 2.f * PI - mPhi;
			mTheta = std::acos(mPosition.y / mR);
		}

		void ComputeDirction()
		{
			mCz = glm::normalize(mPosition - mLookAt); // -1 * dir in raytracer
			mCx = glm::normalize(glm::cross(mUp, mCz));
			mCy = glm::cross(mCz, mCx);
		}

		void ComputeProjMatrix()
		{
			mProjMatrix = glm::perspective(mFovy, (float)mWidth / (float)mHeight, mNear, mFar);
		}
	};
}
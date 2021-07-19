#pragma once
#include "common/Core.h"
#include "math/MathUtils.h"
#include "math/Lingal.h"

namespace Visual {

	struct Distribution1D
	{
	private:
		friend struct Distribution2D;
		std::vector<real> mFunc, mCDF;
		real mFuncInt;
		int mCount;

	public:
		Distribution1D(const real* f, int n)
		{
			mCount = n;
			mFunc.resize(n);
			memcpy(&mFunc[0], f, n * sizeof(real));
			mCDF.resize(n + 1);

			// Compute integral of step function at $x_i$
			mCDF[0] = 0.;
			for (int i = 1; i < mCount + 1; ++i)
				mCDF[i] = mCDF[i - 1] + mFunc[i - 1] / n;

			// Transform step function integral into CDF
			mFuncInt = mCDF[mCount];
			if (mFuncInt == 0.f)
			{
				for (int i = 1; i < n + 1; ++i)
					mCDF[i] = real(i) / real(n);
			}
			else
			{
				for (int i = 1; i < n + 1; ++i)
					mCDF[i] /= mFuncInt;
			}
		}

		~Distribution1D()
		{
		}

		real SampleContinuous(real u, real* pdf, int* off = NULL) const
		{
			auto it = std::upper_bound(mCDF.begin(), mCDF.begin() + mCount + 1, u);
			int offset = Clamp(int(std::distance(mCDF.begin(), it) - 1), 0, mCount - 1);
			if (off) *off = offset;
			GYT_ASSERT(offset < mCount);
			GYT_ASSERT(u >= mCDF[offset] && (u < mCDF[offset + 1] || u == 1));

			// Fix the case when func ends with zeros
			if (mCDF[offset] == mCDF[offset + 1])
			{
				GYT_ASSERT(u == 1.0f);

				do { offset--; } while (mCDF[offset] == mCDF[offset + 1] && offset > 0);

				GYT_ASSERT(mCDF[offset] != mCDF[offset + 1]);
			}

			// Compute offset along CDF segment
			real du = (u - mCDF[offset]) / (mCDF[offset + 1] - mCDF[offset]);
			GYT_ASSERT(!std::isnan(du));

			// Compute PDF for sampled offset
			if (pdf) *pdf = mFunc[offset] / mFuncInt;
			GYT_ASSERT(mFunc[offset] > 0);

			// Return $x\in{}[0,1]$ corresponding to sample
			return (offset + du) / mCount;
		}

		int SampleDiscrete(real u, real* pdf) const
		{
			// Find surrounding CDF segments and _offset_
			auto it = std::upper_bound(mCDF.begin(), mCDF.begin() + mCount + 1, u);
			int offset = std::max(0, int(std::distance(mCDF.begin(), it)) - 1);
			GYT_ASSERT(offset < mCount);
			GYT_ASSERT(u >= mCDF[offset] && u < mCDF[offset + 1]);
			if (pdf) *pdf = mFunc[offset] / (mFuncInt * mCount);
			return offset;
		}

	};

	struct Distribution2D
	{
	private:
		std::vector<Distribution1D*> mpConditionalV;
		Distribution1D* mpMarginal;
	public:
		Distribution2D(const real* func, int nu, int nv)
		{
			mpConditionalV.reserve(nv);
			for (int v = 0; v < nv; ++v)
			{
				// Compute conditional sampling distribution for $\tilde{v}$
				mpConditionalV.push_back(new Distribution1D(&func[v * nu], nu));
			}

			// Compute marginal sampling distribution $p[\tilde{v}]$
			std::vector<real> marginalFunc;
			marginalFunc.reserve(nv);
			for (int v = 0; v < nv; ++v)
				marginalFunc.push_back(mpConditionalV[v]->mFuncInt);
			mpMarginal = new Distribution1D(&marginalFunc[0], nv);
		}

		~Distribution2D()
		{
			delete mpMarginal;
			for (uint32_t i = 0; i < mpConditionalV.size(); ++i)
				delete mpConditionalV[i];
		}

		void SampleContinuous(real u0, real u1, real uv[2], real* pdf) const
		{
			real pdfs[2];
			int v;
			uv[1] = mpMarginal->SampleContinuous(u1, &pdfs[1], &v);
			uv[0] = mpConditionalV[v]->SampleContinuous(u0, &pdfs[0]);
			*pdf = pdfs[0] * pdfs[1];
		}

		real Pdf(real u, real v) const
		{
			int iu = Clamp((int)(u * mpConditionalV[0]->mCount), 0,
				mpConditionalV[0]->mCount - 1);
			int iv = Clamp((int)(v * mpMarginal->mCount), 0,
				mpMarginal->mCount - 1);
			if (mpConditionalV[iv]->mFuncInt * mpMarginal->mFuncInt == 0.f) return 0.f;
			return (mpConditionalV[iv]->mFunc[iu] * mpMarginal->mFunc[iv]) /
				(mpConditionalV[iv]->mFuncInt * mpMarginal->mFuncInt);
		}
	};


}


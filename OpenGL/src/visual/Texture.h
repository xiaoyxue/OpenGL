#pragma once

namespace Visual {
																				template<typename T>
	//class Texture {
	//public:
	//	Texture() = default;

	//	virtual ~Texture() { }

	//	virtual T Sample(const Vec3& coord) const {
	//		return T(0);
	//	}

	//	virtual T Sample(const Vec2& coord) const {
	//		return T(0);
	//	}
	//};


	//template<typename T>
	//class ConstantTexture : public Texture<T> {
	//public:
	//	ConstantTexture(T value) : value(value) {}

	//	T Sample(const Vec3& coord) const override {
	//		return value;
	//	}

	//	T Sample(const Vec2& coord) const override {
	//		return value;
	//	}

	//	T Sample(const Intersection& coord) const override {
	//		return value;
	//	}

	//private:
	//	T value;
	//};

	//enum class WrapMode {
	//	REPEAT, BLACK, CLAMP
	//};

	//enum class FilterMode {
	//	NEAREST,
	//	BILINEAR
	//};


	//template<typename T>
	//class ImageTexture : public Texture<T>
	//{
	//public:
	//	Array2D<T> mTexture;
	//	const WrapMode mWrapMode;
	//	const FilterMode mFilterMode;
	//public:
	//	ImageTexture(const Array2D<T>& image, WrapMode wrapMode = WrapMode::CLAMP, FilterMode filterMode = FilterMode::BILINEAR) :
	//		mTexture(image), mWrapMode(wrapMode), mFilterMode(filterMode) {
	//	}

	//	ImageTexture(const std::string& filename, WrapMode wrapMode = WrapMode::CLAMP, FilterMode filterMode = FilterMode::BILINEAR) :
	//		mWrapMode(wrapMode), mFilterMode(filterMode) {
	//		mTexture = ImageIO::LoadTexture(filename);
	//	}

	//	T Sample(const Vec2& coord) const override {
	//		int resX = mTexture.res[1];
	//		int resY = mTexture.res[0];
	//		if (mFilterMode == FilterMode::BILINEAR) {
	//			return Triangle(coord[0], coord[1]);
	//		}
	//		else {
	//			return Nearest(coord[0], coord[1]);
	//		}
	//	}

	//	T Sample(const Intersection& isect) const override {
	//		return Sample(isect.mUV);
	//	}

	//	T Texel(int u, int v) const {
	//		if (mWrapMode == WrapMode::CLAMP) {
	//			u = Clamp(u, 0, mTexture.res[1] - 1);
	//			v = Clamp(v, 0, mTexture.res[0] - 1);
	//		}
	//		return mTexture[v][u];
	//	}

	//	T Triangle(float u, float v) const {
	//		int resX = mTexture.res[1];
	//		int resY = mTexture.res[0];
	//		float s = resX * u - 0.5f;
	//		float t = resY * v - 0.5f;
	//		int s0 = std::floor(s), t0 = std::floor(t);
	//		float ds = s - s0, dt = t - t0;
	//		T texel = Lerp(dt,
	//			Lerp(ds, Texel(s0, t0), Texel(s0 + 1, t0)),
	//			Lerp(ds, Texel(s0, t0 + 1), Texel(s0 + 1, t0 + 1)));
	//		return texel;
	//	}

	//	T Nearest(float u, float v) const {
	//		int resX = mTexture.res[1];
	//		int resY = mTexture.res[0];
	//		T texel = Texel(int(u * resX), int(v * resY));
	//		return texel;
	//	}

	//	int Height() const {
	//		return mTexture.res[0];
	//	}

	//	int Width() const {
	//		return mTexture.res[1];
	//	}

	//	Vec3 ElementAt(int i, int j) const {
	//		return mTexture[mTexture.res[0] - j - 1][i];
	//	}
	//};
}
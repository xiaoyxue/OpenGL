#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "math/Lingal.h"
#include "math/AABB.h"
namespace OpenGL
{
	using namespace Math;
	class PointCloud
	{
	public:
		std::vector<float> mVertices;
		std::vector<unsigned int> mGLIndices;
		unsigned int mVerticesCount = 0;
		BBox mBBox;

		void Print() {
			for (auto e : mVertices) {
				std::cout << e << std::endl;
			}
		}
		void Load(const std::string& inputfile);
	};
}

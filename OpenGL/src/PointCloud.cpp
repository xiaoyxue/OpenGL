#include "PointCloud.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace OpenGL
{
	using namespace Math;
	void PointCloud::Load(const std::string& inputfile)
	{
		std::ifstream file;
		file.open(inputfile);
		std::string line;
		
		while (std::getline(file, line)) {
			std::stringstream ss(line);
			float x, y, z, r, g, b;
			ss >> x >> y >> z >> r >> g >> b;
			mVertices.push_back(x); mVertices.push_back(y); mVertices.push_back(z);
			mVertices.push_back(r); mVertices.push_back(g); mVertices.push_back(b);
			mBBox.Fit(Vec3(x, y, z));
			mGLIndices.push_back(mVerticesCount++);
		}
	}

}


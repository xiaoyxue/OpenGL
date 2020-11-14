#pragma once
#include "math/Lingal.h"
#include <vector>
#include <string>

namespace OpenGL
{
	using namespace Math;
	class ObjMesh
	{
	public:
		std::vector<float> mGLVertices;
		std::vector<unsigned int> mGLIndices;
		std::vector<Vec3> mVertices;
		std::vector<Vec2> mTextureCoords;
		std::vector<Vec3> mNormals;
		std::string mFileName;

	public:
		ObjMesh();
		~ObjMesh();
		void Load(const std::string& inputfile);
		inline std::vector<float>& GetGLVertices() { return mGLVertices; }
		inline std::vector<unsigned int>& GetGLIndex() { return mGLIndices; }
	};
}
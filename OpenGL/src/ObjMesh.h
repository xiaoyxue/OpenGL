#pragma once
#include "glm/glm.hpp"
#include <vector>
#include <string>

namespace OpenGL
{
	class ObjMesh
	{
		using vec3 = glm::vec3;
		using vec2 = glm::vec2;
	public:
		std::vector<float> mGLVertices;
		std::vector<unsigned int> mGLIndices;
		std::vector<vec3> mVertices;
		std::vector<vec2> mTextureCoords;
		std::vector<vec3> mNormals;
		std::string mFileName;

	public:
		ObjMesh();
		~ObjMesh();
		void Load(const std::string& inputfile);
		inline std::vector<float>& GetGLVertices() { return mGLVertices; }
		inline std::vector<unsigned int>& GetGLIndex() { return mGLIndices; }
	};
}
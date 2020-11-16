#include "ObjMesh.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader/tiny_obj_loader.h"
#include <iostream>

namespace OpenGL
{

	ObjMesh::ObjMesh()
	{

	}

	ObjMesh::~ObjMesh()
	{

	}

	void ObjMesh::Load(const std::string& inputfile)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn;
		std::string err;

		mFileName = inputfile;
		float lineColor[3] = { 0.f, 0.f, 0.3f };

		bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str());

		//Add default material
		materials.push_back(tinyobj::material_t());

		if (!warn.empty()) {
			std::cout << warn << std::endl;
		}

		if (!err.empty()) {
			std::cerr << err << std::endl;
		}

		if (!ret) {
			exit(1);
		}

		unsigned int nVertices = 0;
		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++) 
		{
			// Loop over faces(polygon)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) 
			{
				int fv = shapes[s].mesh.num_face_vertices[f];

				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++) 
				{
					// access to vertex
					tinyobj::real_t 
						vx, vy, vz,
						nx, ny, nz,
						tx, ty;
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					if (idx.vertex_index != -1)
					{
						vx = attrib.vertices[3 * idx.vertex_index + 0];
						vy = attrib.vertices[3 * idx.vertex_index + 1];
						vz = attrib.vertices[3 * idx.vertex_index + 2];
						Vec3 vertex(vx, vy, vz);
						mVertices.push_back(vertex);
						mBBox.Fit(vertex);
					}
					if (idx.normal_index != -1)
					{
						nx = attrib.normals[3 * idx.normal_index + 0];
						ny = attrib.normals[3 * idx.normal_index + 1];
						nz = attrib.normals[3 * idx.normal_index + 2];
						mNormals.push_back(Vec3(nx, ny, nz));
						mHasNormal = true;
					}
					if (idx.texcoord_index != -1)
					{
						tx = attrib.texcoords[2 * idx.texcoord_index + 0];
						ty = attrib.texcoords[2 * idx.texcoord_index + 1];
						mTextureCoords.push_back(Vec2(tx, ty));
					}
					else
					{
						mTextureCoords.push_back(Vec2(0, 0));
					}
					// Optional: vertex colors
					// tinyobj::real_t red = attrib.colors[3*idx.vertex_index+0];
					// tinyobj::real_t green = attrib.colors[3*idx.vertex_index+1];
					// tinyobj::real_t blue = attrib.colors[3*idx.vertex_index+2];

					mGLIndices.push_back(nVertices);
					nVertices++;
				}
				if (!mHasNormal) {
					Vec3* a = &mVertices[mVertices.size() - 1] - 2;
					Vec3 generatedNormal = Cross(a[1] - a[0], a[2] - a[0]);
					if (generatedNormal.Length() > 1e-6f) {
						generatedNormal = generatedNormal.Norm();
					}
					for (int v = 0; v < fv; v++) {
						mNormals.push_back(generatedNormal);
					}
				}
				for (int v = 0; v < fv; ++v)
				{
					float normal_factor = 0.2f;
					float diffuse_factor = 1 - normal_factor;
					float diffuse[3];
					for (int i = 0; i < 3; ++i)
						diffuse[i] = materials[materials.size() - 1].diffuse[i];
					float c[3] = { mNormals[nVertices - fv + v][0] * normal_factor + diffuse[0] * diffuse_factor,
								   mNormals[nVertices - fv + v][1] * normal_factor + diffuse[1] * diffuse_factor,
								   mNormals[nVertices - fv + v][2] * normal_factor + diffuse[2] * diffuse_factor };
					float len2 = c[0] * c[0] + c[1] * c[1] + c[2] * c[2];
					if (len2 > 0.f)
					{
						float len = std::sqrt(len2);
						c[0] /= len;
						c[1] /= len;
						c[2] /= len;
					}
					mGLVertices.push_back(mVertices[nVertices - fv + v].x);
					mGLVertices.push_back(mVertices[nVertices - fv + v].y);
					mGLVertices.push_back(mVertices[nVertices - fv + v].z);
					mGLVertices.push_back(mNormals[nVertices - fv + v].x);
					mGLVertices.push_back(mNormals[nVertices - fv + v].y);
					mGLVertices.push_back(mNormals[nVertices - fv + v].z);
					mGLVertices.push_back(mTextureCoords[nVertices - fv + v].x);
					mGLVertices.push_back(mTextureCoords[nVertices - fv + v].y);
					mGLVertices.push_back(c[0] * 0.5 + 0.5);
					mGLVertices.push_back(c[1] * 0.5 + 0.5);
					mGLVertices.push_back(c[2] * 0.5 + 0.5);
					mGLVertices.push_back(lineColor[0]);
					mGLVertices.push_back(lineColor[1]);
					mGLVertices.push_back(lineColor[2]);
				}
				index_offset += fv;

				// per-face material
				//shapes[s].mesh.material_ids[f];
			}
		}
	}

}


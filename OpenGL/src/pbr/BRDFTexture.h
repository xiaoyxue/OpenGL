#pragma once
#include "ForwardDecl.h"
#include "opengl/GLOjbect.h"
#include "opengl/Texture.h"

namespace PBR {

	using namespace OpenGL;
	class BRDFTexture : public Texture2D
	{

	public:
		BRDFTexture() = default;
		BRDFTexture(int width, int height);
		~BRDFTexture();
	};
}


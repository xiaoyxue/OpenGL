#pragma once
#include "ForwardDecl.h"
#include "opengl/GLOjbect.h"
#include "opengl/Texture.h"

namespace PBR {

	using namespace OpenGL;
	class BRDFTexture : public Texture
	{

	public:
		BRDFTexture();
		BRDFTexture(int width, int height);
		~BRDFTexture();
	};
}


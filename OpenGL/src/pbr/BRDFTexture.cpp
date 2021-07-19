#include "BRDFTexture.h"
#include "opengl/Texture.h"
#include "opengl/OpenGL.h"
#include "opengl/GLImage.h"
#include "stb/stb_image.h"


namespace PBR 
{

	BRDFTexture::BRDFTexture(int width, int height): Texture(width, height)
	{
		//unsigned int brdfLUTTexture;
		GLCall(glGenTextures(1, &mHandle));
		// pre-allocate enough memory for the LUT texture.
		GLCall(glBindTexture(GL_TEXTURE_2D, mHandle));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, mWidth, mHeight, 0, GL_RG, GL_FLOAT, 0));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}


	BRDFTexture::~BRDFTexture()
	{
	}

}
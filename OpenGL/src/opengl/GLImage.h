#pragma once
#include "OpenGL.h"

namespace OpenGL
{
	enum class ImageDataType
	{
		Byte = GL_UNSIGNED_BYTE, Float = GL_FLOAT, UnsignedInt = GL_UNSIGNED_INT_8_8_8_8
	};

	enum class ImageFormat
	{
		Luminance = GL_LUMINANCE, R = GL_R, RG = GL_RG, RGB = GL_RGB, RGBA = GL_RGBA,
		Red = GL_RED,
		RGBA_32F = GL_RGBA32F, RGBA_16F = GL_RGBA16F,
		R11_G11_B10 = GL_R11F_G11F_B10F,
		RGB10_A2 = GL_RGB10_A2,
		Depth16 = GL_DEPTH_COMPONENT16,
		Depth24 = GL_DEPTH_COMPONENT24,
		Depth32 = GL_DEPTH_COMPONENT32,
		DXT1_RGB = GL_COMPRESSED_RGB_S3TC_DXT1_EXT,
		DXT1_RGBA = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,
		DXT3_RGBA = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,
		DXT5_RGBA = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,
		RGBA_INT = GL_BGRA_INTEGER
	};
}

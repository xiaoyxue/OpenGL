#pragma once
#include "ForwardDecl.h"
#include "OpenGL.h"
#include "GLOjbect.h"


namespace OpenGL
{
	enum class FrameBufferTarget
	{
		Draw = GL_DRAW_FRAMEBUFFER,
		Read = GL_READ_FRAMEBUFFER,
		Frame = GL_FRAMEBUFFER
	};

	enum class FrameBufferAttachment
	{
		Color0 = GL_COLOR_ATTACHMENT0,
		Color1 = GL_COLOR_ATTACHMENT1,
		Color2 = GL_COLOR_ATTACHMENT2,
		Color3 = GL_COLOR_ATTACHMENT3,
		Color4 = GL_COLOR_ATTACHMENT4,
		Color5 = GL_COLOR_ATTACHMENT5,
		Color6 = GL_COLOR_ATTACHMENT6,
		Color7 = GL_COLOR_ATTACHMENT7,
		Color8 = GL_COLOR_ATTACHMENT8,
		Color9 = GL_COLOR_ATTACHMENT9,
		Color10 = GL_COLOR_ATTACHMENT10,
		Color11 = GL_COLOR_ATTACHMENT11,
		Color12 = GL_COLOR_ATTACHMENT12,
		Color13 = GL_COLOR_ATTACHMENT13,
		Color14 = GL_COLOR_ATTACHMENT14,
		Color15 = GL_COLOR_ATTACHMENT15,
		Depth = GL_DEPTH_ATTACHMENT,
		Stencil = GL_STENCIL_ATTACHMENT,
		Depth24 = GL_DEPTH_COMPONENT24
	};

	class RenderBuffer : public GLObject
	{
	public:
		RenderBuffer();
		virtual ~RenderBuffer();
	public:
		void SetStorage(int w, int h, ImageFormat format, int multiSampleCount = 0);
		void Bind() const;
		void UnBind() const;
	};

	class FrameBuffer : public GLObject
	{
	private:
		int mTarget;
	public:
		FrameBuffer();
		virtual ~FrameBuffer();
	public:
		void SetTarget(FrameBufferTarget target);
		void Attach(FrameBufferAttachment attachment, RenderBuffer* renderBuffer);
		void Attach(FrameBufferAttachment attachment, Texture2D* texture, int level = 0);
		void Bind() const;
		void UnBind() const;
		void Check();
	};
}

#include "FrameBuffer.h"
#include "Texture.h"
#include "GLImage.h"
#include <iostream>

namespace OpenGL
{
	RenderBuffer::RenderBuffer()
	{
		glGenRenderbuffers(1, &mHandle);
	}

	RenderBuffer::~RenderBuffer()
	{
		glDeleteRenderbuffers(1, &mHandle);
	}

	void RenderBuffer::SetStorage(int w, int h, ImageFormat format, int multiSampleCount)
	{
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, mHandle));
		GLCall(glRenderbufferStorageMultisample(GL_RENDERBUFFER, multiSampleCount, (int)format, w, h));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
	}

	void RenderBuffer::Bind() const
	{
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, mHandle));
	}

	void RenderBuffer::UnBind() const
	{
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
	}

	FrameBuffer::FrameBuffer()
	{
		mTarget = (int)FrameBufferTarget::Frame;
		GLCall(glGenFramebuffers(1, &mHandle));
	}

	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffers(1, &mHandle);
	}

	void FrameBuffer::Attach(FrameBufferAttachment attachment, RenderBuffer* renderBuffer)
	{
		Bind();
		GLCall(glFramebufferRenderbuffer(mTarget, (int)attachment, GL_RENDERBUFFER, renderBuffer->GetHandle()));
		UnBind();
	}

	void FrameBuffer::Attach(FrameBufferAttachment attachment, Texture2D* texture, int level)
	{
		Bind();
		GLCall(glFramebufferTexture2D(mTarget, (int)attachment, GL_TEXTURE_2D, texture->GetHandle(), level));
		UnBind();
	}

	void FrameBuffer::SetTarget(FrameBufferTarget target)
	{
		this->mTarget = (int)target;
	}

	void FrameBuffer::Bind() const
	{
		GLCall(glBindFramebuffer(mTarget, mHandle));
	}

	void FrameBuffer::UnBind() const
	{
		GLCall(glBindFramebuffer(mTarget, 0));
	}

	void FrameBuffer::Check()
	{
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}

}


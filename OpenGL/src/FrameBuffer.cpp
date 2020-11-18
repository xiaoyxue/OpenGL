#include "FrameBuffer.h"
#include "Texture.h"
#include "GLImage.h"

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
		glBindRenderbuffer(GL_RENDERBUFFER, mHandle);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, multiSampleCount, (int)format, w, h);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	FrameBuffer::FrameBuffer()
	{
		mTarget = (int)FrameBufferTarget::Draw;
		glGenFramebuffers(1, &mHandle);
	}

	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffers(1, &mHandle);
	}

	void FrameBuffer::Attach(FrameBufferAttachment attachment, RenderBuffer* renderBuffer)
	{
		Bind();
		glFramebufferRenderbuffer(mTarget, (int)attachment, GL_RENDERBUFFER, renderBuffer->GetHandle());
		UnBind();
	}

	void FrameBuffer::Attach(FrameBufferAttachment attachment, Texture* texture, int level)
	{
		Bind();
		glFramebufferTexture2D(mTarget, (int)attachment, GL_TEXTURE_2D, texture->GetHandle(), level);
		UnBind();
	}

	void FrameBuffer::SetTarget(FrameBufferTarget target)
	{
		this->mTarget = (int)target;
	}

	void FrameBuffer::Bind()
	{
		glBindFramebuffer(mTarget, mHandle);
	}

	void FrameBuffer::UnBind()
	{
		glBindFramebuffer(mTarget, 0);
	}
}


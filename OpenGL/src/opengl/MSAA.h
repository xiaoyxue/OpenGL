#pragma once
#include "OpenGL.h"
#include "FrameBuffer.h"
namespace OpenGL {

	class MSAA {
	public:
		static void Blit(const FrameBuffer& src, const FrameBuffer& dest, int w, int h) {
			GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, src.GetHandle()));
			GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dest.GetHandle()));
			GLCall(glBlitFramebuffer(0, 0, w, h, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_LINEAR));
		}
	};
}

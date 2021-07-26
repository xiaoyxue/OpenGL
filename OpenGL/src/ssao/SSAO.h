#pragma once
#include "opengl/GLOjbect.h"
#include "opengl/FrameBuffer.h"
#include "opengl/DrawableObject.h"
#include "opengl/FrameBuffer.h"
#include "opengl/Texture.h"
#include "opengl/GLImage.h"
#include <random>


namespace OpenGL 
{
	class SSAO {
	public:
		std::unique_ptr<Texture2D> mpPosition;
		std::unique_ptr<Texture2D> mpNormal;
		std::unique_ptr<Texture2D> mpAlbedo;
		std::unique_ptr<Texture2D> mpSSAO_Color;
		std::unique_ptr<Texture2D> mpSSAO_BlurColor;
		std::unique_ptr<RenderBuffer> mpRenderBuffer;

	public:
		FrameBuffer mFrameBuffer;
		FrameBuffer mSSAO_Buffer;
		FrameBuffer mSSAO_BlurBuffer;
	public:
		SSAO() = delete;
		SSAO(int resolutionX, int resolutionY) 
		{
			//G buffer
			mpPosition = std::make_unique<Texture2D>(resolutionX, resolutionY, ImageFormat::RGBA, ImageDataType::Float, ImageFormat::RGBA_16F);
			mpNormal = std::make_unique<Texture2D>(resolutionX, resolutionY, ImageFormat::RGBA, ImageDataType::Float, ImageFormat::RGBA_16F);
			mpAlbedo = std::make_unique<Texture2D>(resolutionX, resolutionY, ImageFormat::RGBA, ImageDataType::Float, ImageFormat::RGBA);


			mFrameBuffer.Bind();
			mFrameBuffer.SetTarget(FrameBufferTarget::Frame);
			mpPosition->Bind();
			mpPosition->SetFilter(TextureFilter::Nearest);
			mFrameBuffer.Attach(FrameBufferAttachment::Color0, mpPosition.get());

			mFrameBuffer.Bind();
			mpNormal->Bind();
			mpNormal->SetFilter(TextureFilter::Nearest);
			mFrameBuffer.Attach(FrameBufferAttachment::Color1, mpNormal.get());

			mFrameBuffer.Bind();
			mpAlbedo->Bind();
			mpAlbedo->SetFilter(TextureFilter::Nearest);
			mFrameBuffer.Attach(FrameBufferAttachment::Color2, mpAlbedo.get());
			unsigned int attachments[3] = {
				(unsigned int)(FrameBufferAttachment::Color0),
				(unsigned int)(FrameBufferAttachment::Color1),
				(unsigned int)(FrameBufferAttachment::Color2) };
			mFrameBuffer.Bind();
			GLCall(glDrawBuffers(3, attachments));

			mpRenderBuffer = std::make_unique<RenderBuffer>();
			mFrameBuffer.Bind();
			mpRenderBuffer->SetStorage(resolutionX, resolutionY, ImageFormat::Depth32, 0);
			mFrameBuffer.Attach(FrameBufferAttachment::Depth, mpRenderBuffer.get());

			mFrameBuffer.Bind();
			mFrameBuffer.Check();

			//SSAO buffer
			mSSAO_Buffer.Bind();
			mSSAO_Buffer.SetTarget(FrameBufferTarget::Frame);
			mpSSAO_Color = std::make_unique<Texture2D>(resolutionX, resolutionY, ImageFormat::RGB, ImageDataType::Float, ImageFormat::RGB);
			mpSSAO_Color->Bind();
			mpSSAO_Color->SetFilter(TextureFilter::Linear);
			mSSAO_Buffer.Attach(FrameBufferAttachment::Color0, mpSSAO_Color.get());
			mSSAO_Buffer.Bind();
			mSSAO_Buffer.Check();

			//SSAO blur buffer
			mSSAO_BlurBuffer.Bind();
			mSSAO_BlurBuffer.SetTarget(FrameBufferTarget::Frame);
			mpSSAO_BlurColor = std::make_unique<Texture2D>(resolutionX, resolutionY, ImageFormat::RGBA, ImageDataType::Float);
			mpSSAO_BlurColor->Bind();
			mSSAO_BlurBuffer.Attach(FrameBufferAttachment::Color0, mpSSAO_BlurColor.get());
			mSSAO_BlurBuffer.Bind();
			mSSAO_BlurBuffer.Check();

		}
	
	public:

		void BindGbuffer() const {
			mFrameBuffer.Bind();
		}

		void UnBindGbuffer() const {
			mFrameBuffer.UnBind();
		}

		void BindSSAO_Buffer() const {
			mSSAO_Buffer.Bind();
		}

		void UnBindSSAO_Buffer() const {
			mSSAO_Buffer.UnBind();
		}

		void BindSSAO_BlurBuffer() const {
			mSSAO_BlurBuffer.Bind();
		}

		void UnBindSSAO_BlurBuffer() const {
			mSSAO_BlurBuffer.UnBind();
		}
	};
}
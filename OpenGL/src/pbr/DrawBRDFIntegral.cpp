#include "DrawBRDFIntegral.h"
#include "opengl/OpenGL.h"
#include "opengl/Texture.h"
#include "opengl/Renderer.h"
#include "visual/Camera.h"

namespace PBR 
{
	

	BRDFIntegral::BRDFIntegral()
	{
		mpVAO = std::make_unique<VertexArray>();
		mpVBO = std::make_unique<VertexBuffer>();
		mpIBO = std::make_unique<IndexBuffer>();
		mpVBO->SetData(&(mPoints[0]), 32 * sizeof(float));
		mpIBO->SetData(&(mIndex[0]), 6);
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);
		mpVAO->AddBuffer(*mpVBO, layout);
		mpVAO->UnBind();
		mpVBO->UnBind();
		mpIBO->UnBind();
	}

	BRDFIntegral::~BRDFIntegral()
	{

	}

	void BRDFIntegral::DrawFace(const Renderer& renderer) const
	{

		unsigned int fbo, rbo;
		GLCall(glGenFramebuffers(1, &fbo));
		GLCall(glGenRenderbuffers(1, &rbo));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, rbo));
		mpTexture->Bind();
		GLuint brdfLUTTexture = mpTexture->GetHandle();
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mpTexture->GetWidth(), mpTexture->GetHeight()));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0));

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
		auto& shader = mShaders["BRDF_LUT"];
		renderer.EnableDepthTest();
		shader->Bind();
		shader->SetUniform2f("u_Resolution", mpTexture->GetWidth(), mpTexture->GetHeight());
		glViewport(0, 0, mpTexture->GetWidth(), mpTexture->GetHeight());
		shader->SetUniform1i("u_Texture", 0); mpTexture->Bind(0);
		renderer.Draw(*mpVAO, *mpIBO, *shader);
		renderer.DisableDepthTest();
		mpTexture->UnBind();
		shader->UnBind();
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void BRDFIntegral::AddShader(const std::string& shaderName, std::shared_ptr<Shader> shader)
	{
		mShaders[shaderName] = shader;
	}

}
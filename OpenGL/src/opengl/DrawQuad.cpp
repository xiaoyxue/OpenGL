#include "DrawQuad.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "ObjMesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "visual/Camera.h"
#include "ssao/SSAO.h"
#include <GLFW/glfw3.h>

namespace OpenGL
{

	float lerp(float a, float b, float f)
	{
		return a + f * (b - a);
	}

	using namespace Math;
	DrawQuad::DrawQuad(int resX, int resY) : mResolution(resX, resY)
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

	DrawQuad::~DrawQuad()
	{

	}

	void DrawQuad::DrawFace(const Renderer& renderer) const
	{
		auto& shader = mShaders["Debug"];
		renderer.EnableDepthTest();
		shader->Bind();
		Matrix4 proj = renderer.GetCamera()->GetProjMatrix();
		Matrix4 view = renderer.GetCamera()->GetViewMatrix();
		Matrix4 model(1);
		shader->SetUniformMat4f("u_Model", model);
		shader->SetUniformMat4f("u_View", view);
		shader->SetUniformMat4f("u_Proj", proj);
		//uniform float iTime;
		shader->SetUniform1f("iTime", glfwGetTime());
		//uniform vec3 camDir;
		const Camera* pCamera = renderer.GetCamera();
		shader->SetUniform3f("camDir", pCamera->mCz.x, pCamera->mCz.y, pCamera->mCz.z);
		//uniform vec3 camPos;
		shader->SetUniform3f("camPos", pCamera->GetPosition().x, pCamera->GetPosition().y, pCamera->GetPosition().z);
		//uniform vec3 camLookAt;
		Vec3 cameraLookAt = pCamera->GetPosition() + pCamera->mCz;
		shader->SetUniform3f("camLookAt", cameraLookAt.x, cameraLookAt.y, cameraLookAt.z);
		//uniform vec3 camUU;
		shader->SetUniform3f("camUU", pCamera->mCx.x, pCamera->mCx.y, pCamera->mCx.z);
		//uniform vec3 camVV;
		shader->SetUniform3f("camVV", pCamera->mCy.x, pCamera->mCy.y, pCamera->mCy.z);
		//uniform vec3 camWW;
		shader->SetUniform3f("camWW", pCamera->mCz.x, pCamera->mCz.y, pCamera->mCz.z);
		shader->SetUniform2f("iResolution", mResolution.x, mResolution.y);
		//shader->SetUniform2f("iMouse", 0, 0);
		//shader->SetUniform1i("u_Texture", 0); mpTexture->Bind(0);
		//shader->SetUniform1i("waterTexture", 1); mpTexture->Bind(1);
		//shader->SetUniform1i("waterBottomTexture", 2); mpTexture->Bind(2);
		shader->SetUniform1i("backgroundTexture", 0); mTextures["backgroundTexture"]->Bind(0);
		shader->SetUniform1i("noiseTexture", 1); mTextures["noiseTexture"]->Bind(1);
		renderer.Draw(*mpVAO, *mpIBO, *shader);
		renderer.DisableDepthTest();
		mpTexture->UnBind();
		shader->UnBind();
	}

	void DrawQuad::DrawSSAO(const Renderer& renderer, const SSAO& ssaoBuffer) const
	{
		ssaoBuffer.BindSSAO_Buffer();

		auto& shader = mShaders["SSAO_Shader"];
		renderer.EnableDepthTest();
		shader->Bind();
		Matrix4 proj = renderer.GetCamera()->GetProjMatrix();
		Matrix4 view = renderer.GetCamera()->GetViewMatrix();
		Matrix4 model = LocalToWorld.GetMatrix();
		shader->SetUniformMat4f("Model", model);
		shader->SetUniformMat4f("View", view);
		shader->SetUniformMat4f("Proj", proj);

		for (int i = 0; i < mSample.size(); ++i) {
			shader->SetUniform3f("samples[" + std::to_string(i) + "]", mSample[i][0], mSample[i][1], mSample[i][2]);
		}

		//for (int i = 0; i < mSample.size(); ++i) {
		//	shader->SetUniform3f("samples[" + std::to_string(i) + "]", ssaoKernel[i][0], ssaoKernel[i][1], ssaoKernel[i][2]);
		//}

		shader->SetUniform1i("gPosition", 0); ssaoBuffer.mpPosition->Bind(0);
		shader->SetUniform1i("gNormal", 1); ssaoBuffer.mpNormal->Bind(1);
		//shader->SetUniform1i("gAlbedo", 2); ssaoBuffer.mpAlbedo->Bind(2);
		//shader->SetUniform1i("texNoise", 2); mpNoiseTexture->Bind(2);
		renderer.Draw(*mpVAO, *mpIBO, *shader);
		renderer.DisableDepthTest();
		shader->UnBind();

		ssaoBuffer.UnBindSSAO_Buffer();
	}

	void DrawQuad::DrawToFrameBuffer(const Renderer& renderer, const FrameBuffer& frameBuffer) const
	{
		auto& shader = mShaders["SSAO_Shader"];
		renderer.EnableDepthTest();
		shader->Bind();
		Matrix4 proj = renderer.GetCamera()->GetProjMatrix();
		Matrix4 view = renderer.GetCamera()->GetViewMatrix();
		Matrix4 model = LocalToWorld.GetMatrix();
		shader->SetUniformMat4f("Model", model);
		shader->SetUniformMat4f("View", view);
		shader->SetUniformMat4f("Proj", proj);
		renderer.Draw(*mpVAO, *mpIBO, *shader);
		renderer.DisableDepthTest();
		shader->UnBind();

		frameBuffer.UnBind();
	}

	void DrawQuad::GenerateSamples(int n)
	{
		std::uniform_real_distribution<float> randomFloats0(0.f, 1.f);
		std::default_random_engine generator0;
		for (int i = 0; i < n; ++i) {
			mSample.push_back(Vec3(randomFloats0(generator0), randomFloats0(generator0), randomFloats0(generator0)));
		}

		std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
		std::default_random_engine generator;

		for (unsigned int i = 0; i < 64; ++i)
		{
			Vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
			sample = sample.Norm();
			sample *= randomFloats(generator);
			float scale = float(i) / 64.0;

			// scale samples s.t. they're more aligned to center of kernel
			scale = lerp(0.1f, 1.0f, scale * scale);
			sample *= scale;
			ssaoKernel.push_back(sample);
		}

		// generate noise texture
		// ----------------------
		
		for (unsigned int i = 0; i < 16; i++)
		{
			glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
			ssaoNoise.push_back(noise);
		}
		
		mpNoiseTexture = std::make_shared<Texture2D>(4, 4, ImageFormat::RGB, ImageDataType::Float, ImageFormat::RGBA_32F, &ssaoNoise[0]);
	}

	void DrawQuad::DebugDraw(const Renderer& renderer) const
	{
		auto& shader = mShaders["Debug"];
		renderer.EnableDepthTest();
		shader->Bind();
		Matrix4 proj = renderer.GetCamera()->GetProjMatrix();
		Matrix4 view = renderer.GetCamera()->GetViewMatrix();
		Matrix4 model(1);
		shader->SetUniformMat4f("u_Model", model);
		shader->SetUniformMat4f("u_View", view);
		shader->SetUniformMat4f("u_Proj", proj);

		float resolutionX = 1280, resolutionY = 1280;
		shader->SetUniform2f("u_Resolution", resolutionX, resolutionY);
		shader->SetUniform1i("offlineTexture", 0); mTextures["offlineTexture"]->Bind(0);
		shader->SetUniform1i("ssao", 1); mTextures["ssao"]->Bind(1);

		renderer.Draw(*mpVAO, *mpIBO, *shader);
		renderer.DisableDepthTest();
		mpTexture->UnBind();
		shader->UnBind();
	}

	Math::BBox DrawQuad::GetBBox() const
	{
		return BBox();
	}

	void DrawQuad::AddShader(const std::string& shaderName, std::shared_ptr<Shader> shader)
	{
		mShaders[shaderName] = shader;
	}

}


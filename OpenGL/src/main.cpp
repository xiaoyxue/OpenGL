#include <iostream>
#include <string>
#include "opengl/Renderer.h"
#include "opengl/VertexBuffer.h"
#include "opengl/IndexBuffer.h"
#include "opengl/VertexArray.h"
#include "opengl/VertexBufferLayout.h"
#include "opengl/Shader.h"
#include "opengl/Texture.h"
#include "visual/Camera.h"
#include "opengl/GLMesh.h"
#include "opengl/Scene.h"
#include "previewer/Previewer.h"
#include "opengl/GLBackground.h"
#include "opengl/GLMeshShading.h"
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"
#include "image/ImageIO.h"
#include "pbr/BRDFTexture.h"
#include "pbr/DrawBRDFIntegral.h"
#include "opengl/DrawQuad.h"
#include "math/Lingal.h"
#include "math/Transform.h"

using namespace Preview;
using namespace PBR;
using namespace OpenGL;



//
//int main(void)
//{
//	int resolutionX = 1280, resolutionY = 900;
//	Previewer previewer("Hello World!", resolutionX, resolutionY);
//	previewer.Init();
//	Picker picker(resolutionX, resolutionY);
//	Renderer renderer(resolutionX, resolutionY);
//	Camera camera;
//	camera.Init(
//		Vec3(0, 0, 30),
//		Vec3(0, 0, 0),
//		Vec3(0, 1, 0),
//		resolutionX,
//		resolutionY,
//		CameraType::Perspective
//	);
//	renderer.SetCamera(&camera);
//	//model and shader
//
//	Scene scene;
//	previewer.SetPicker(&picker);
//	previewer.SetScene(&scene);
//
//	std::string texturePath = "..\\models\\mailbox\\Mailbox.jpg";
//	//std::string envMapPath = "res\\envmap\\MicrosoftTeams-image.png";
//	std::string envMapPath = "res\\envmap\\prefiltered_kernal_gassian_6.png";
//
//	GLMeshShading mesh;
//	GLTexture texture(texturePath);
//	GLTexture evnMap(envMapPath);
//	mesh.AddMesh("..\\models\\mailbox\\10556_Mailbox-L2.obj");
//	mesh.AddTexture(&texture);
//	mesh.AddTexture(&evnMap);
//	auto faceShader = std::make_shared<Shader>("res/shaders/ImageBasedLighting.shader");
//	//auto faceShader = std::make_shared<Shader>("res/shaders/DefaultFace.shader");
//	auto lineShader = std::make_shared<Shader>("res/shaders/DefaultLine.shader");
//
//	mesh.AddShader("Face", faceShader);
//	mesh.AddShader("WireFrame", lineShader);
//
//
//	//GLBackground background;
//	//background.AddTexture(&evnMap);
//	//auto backgroundShader = std::make_shared<Shader>("res/shaders/DefaultBackground.shader");
//	//background.AddShader("Background", backgroundShader);
//	//previewer.AddDrawableObject(&background);
//
//	previewer.AddDrawableObject(&mesh);
//	previewer.SetCamera(&camera);
//	previewer.SetRenderer(&renderer);
//	// Set max fps
//	previewer.SetMaxFps(60);
//	previewer.MainLoop();
//
//    return 0;
//}




int main(void)
{
	int resolutionX = 1280, resolutionY = 1280;
	Previewer previewer("Hello World!", resolutionX, resolutionY);
	previewer.Init();
	Picker picker(resolutionX, resolutionY);
	Renderer renderer(resolutionX, resolutionY);
	Camera camera;
	camera.Init(
		Vec3(0, 0.5, 5),
		Vec3(0, 0, 0),
		Vec3(0, 1, 0),
		resolutionX,
		resolutionY,
		CameraType::Perspective
	);
	renderer.SetCamera(&camera);
	//model and shader

	Scene scene;
	previewer.SetPicker(&picker);
	previewer.SetScene(&scene);

	//std::string texturePath = "..\\models\\mailbox\\Mailbox.jpg";
	//std::string envMapPath = "res\\envmap\\MicrosoftTeams-image.png";
	//std::string envMapPath = "res\\envmap\\prefiltered_kernal_gassian_3.png";

	//GLMeshShading mesh;
	//GLTexture texture(texturePath);
	//Texture2D evnMap(envMapPath);

	//mesh.AddMesh("..\\models\\mailbox\\10556_Mailbox-L2.obj");
	//mesh.AddTexture(&texture);
	//mesh.AddTexture(&evnMap);
	//auto faceShader = std::make_shared<Shader>("res/shaders/ImageBasedLighting.shader");
	//auto faceShader = std::make_shared<Shader>("res/shaders/DefaultFace.shader");
	//auto lineShader = std::make_shared<Shader>("res/shaders/DefaultLine.shader");

	//mesh.AddShader("Face", faceShader);
	//mesh.AddShader("WireFrame", lineShader);

	//BRDFIntegral brdfIntegral;
	//BRDFTexture brdfLUT(resolutionX, resolutionY);

	//brdfIntegral.AddTexture(&brdfLUT);
	//auto brdfIntegralShader = std::make_shared<Shader>("res/shaders/BRDF_Integral.shader");
	//brdfIntegral.AddShader("BRDF_LUT", brdfIntegralShader);
	//brdfIntegral.DrawFace(renderer);
	




	auto lineShader = std::make_shared<Shader>("res/shaders/DefaultLine.shader");
	auto textureShader = std::make_shared<Shader>("res/shaders/DefaultTexture.shader");
	auto mainboxShader = std::make_shared<Shader>("res/shaders/ImageBasedLightingV2.shader");
	auto gBufferShader = std::make_shared<Shader>("res/shaders/ssao/Gbuffer.shader");
	auto ssaoBufferShader = std::make_shared<Shader>("res/shaders/ssao/SSAO.shader");
	auto debugShader = std::make_shared<Shader>("res/shaders/Debug.shader");
	auto quadShader = std::make_shared<Shader>("res/shaders/water/BasicWater.shader");
	auto waterShader = std::make_shared<Shader>("res/shaders/water/Water1.shader");
	//auto waterShader = std::make_shared<Shader>("res/shaders/water/BasicWater.shader");

	DrawQuad quad;
	std::string planeTexturePath = "res/textures/plane.png";
	Texture2D planeTexture(planeTexturePath);
	//quad.SetTexture("u_Texture", &planeTexture);
	quad.AddTexture(&planeTexture);
	quad.AddShader("Debug", waterShader);
	previewer.SetQuad(&quad);
	previewer.mDrawQuad = true;
	//previewer.SetQuad(&quad);
	//GLMesh plane;
	//std::string planeTexturePath = "..res/textures/plane.png";
	//Texture2D planeTexture(planeTexturePath);
	//previewer.AddDrawableObject(&plane);
	

	previewer.SetCamera(&camera);
	previewer.SetRenderer(&renderer);
	//previewer.SetSSAO();
	// Set max fps
	previewer.SetMaxFps(60);
	previewer.MainLoop();

	return 0;
}



// 
// 
// 
// 
// 
//
//class Filter
//{
//protected:
//	real radius;
//
//public:
//	Filter(const real rad)
//		: radius(rad)
//	{
//	}
//	virtual ~Filter() {}
//
//	const real GetRadius() const
//	{
//		return radius;
//	}
//	virtual real Evaluate(const real dx, const real dy) const = 0;
//};
//
//class BoxFilter : public Filter
//{
//public:
//	BoxFilter()
//		: Filter(0.5)
//	{
//	}
//
//public:
//	real Evaluate(const real dx, const real dy) const override {
//		return 1.0;
//	}
//};
//
//
//class GaussianFilter : public Filter
//{
//private:
//	real stdDev;
//	real alpha;
//	real expR;
//
//public:
//	GaussianFilter(const real stdDiv = 0.9f)
//		: Filter(4 * stdDiv)
//		, stdDev(stdDiv)
//	{
//		alpha = -1.0f / (2.0f * stdDev * stdDev);
//		expR = std::exp(alpha * radius * radius);
//	}
//
//public:
//	real Evaluate(const real dx, const real dy) const override
//	{
//		auto Gaussian = [this](const real d)
//		{
//			return std::max(0.0f, std::exp(alpha * d * d) - expR);
//		};
//
//		return Gaussian(dx) * Gaussian(dy);
//	}
//};
//
//
//Array2D<Vec3> Prefilter(const Array2D<Vec3>& image, int kernalLevel = 0) {
//	int kernalSize = std::pow(2, kernalLevel);
//	Filter *filter = new GaussianFilter(20);
//	std::vector<Vec3> prefilteredData(image.res[0] * image.res[1]);
//	Array2D<Vec3> prefilteredImage(prefilteredData, image.res[0], image.res[1]);
//	for (int y = 0; y < image.res[0]; ++y) {
//		for (int x = 0; x < image.res[1]; ++x) {
//			Vec3 value;
//			real weight = 0.f;
//			for (int i = y - kernalSize; i <= y + kernalSize; ++i) {
//				for (int j = x - kernalSize; j <= x + kernalSize; ++j) {
//					if (i < 0 || i >= image.res[0] || j < 0 || j >= image.res[1])
//						continue;
//					real filterValue = filter->Evaluate(std::abs(i - y), std::abs(j - x));
//					//std::cout << filterValue << std::endl;
//					value += filterValue * image(i, j);
//					weight += filterValue;
//				}
//			} 
//			value /= weight;
//			prefilteredImage(y, x) = value;
//			//prefilteredImage(y, x) = image(y, x);
//		}
//	}
//	return prefilteredImage;
//}
//
//int main(void)
//{
//	std::string filename = "C:\\Users\\xiaoyxue\\Desktop\\RealTime Shading\\RealShading\\RealShading\\res\\envmap\\Origin.png";
//	std::string fileToRight = "C:\\Users\\xiaoyxue\\Desktop\\OpenGL\\output\\prefiltered_kernal_gassian_6.png";
//	int resX, resY, channels;
//	float* data = stbi_loadf(filename.c_str(), &resX, &resY, &channels, 0);
//	Array2D<Vec3> image = OpenGL::ImageIO::LoadImage(filename);
//	std::vector<Vec3> imageToWright;
//	for (int y = 0; y < image.res[0]; ++y) {
//		for (int x = 0; x < image.res[1]; ++x) {
//			imageToWright.push_back(image(y, x));
//		}
//	}
//
//
//	//Filter* filter = new GaussianFilter(30);
//	//std::cout << filter->Evaluate(64, 64);
//	//std::vector<Vec3> prefilteredData(resX * resY);
//	//Array2D<Vec3> prefilteredImage(prefilteredData, resY, resX);
//	//for (int y = 0; y < prefiltered.res[0]; ++y) {
//	//	for (int x = 0; x < prefiltered.res[1]; ++x) {
//	//		prefilteredImage(y, x) = prefiltered(y, x);
//	//	}
//	//}
//
//	Array2D<Vec3> prefiltered = Prefilter(image, 6);
//	OpenGL::ImageIO::WriteImage(fileToRight, prefiltered.GetData(), resX, resY);
//
//	return 0;
//}

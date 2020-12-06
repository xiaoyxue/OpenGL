#include <iostream>
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

using namespace Preview;

int main(void)
{
	int resolutionX = 1280, resolutionY = 900;
	Previewer previewer("Hello World!", resolutionX, resolutionY);
	previewer.Init();
	Picker picker(resolutionX, resolutionY);
	Renderer renderer(resolutionX, resolutionY);
	Camera camera;
	camera.Init(
		Vec3(0, 0, 3),
		Vec3(0, 0, 0),
		Vec3(0, 1, 0),
		resolutionX,
		resolutionY,
		CameraType::Orthographic
	);
	renderer.SetCamera(&camera);
	//model and shader

	Scene scene;
	previewer.SetPicker(&picker);
	previewer.SetScene(&scene);

	std::shared_ptr<Shader> pBackgroundShader0 = std::make_shared<Shader>("res/shaders/DefaultBackground.shader");
	GLBackground background0;
	Texture backgroundTexture0("res/textures/background0.png");
	background0.AddTexture(&backgroundTexture0);
	background0.AddShader("Background", pBackgroundShader0);

	std::shared_ptr<Shader> pBackgroundShader1 = std::make_shared<Shader>("res/shaders/DefaultBackground.shader");
	GLBackground background1;
	Texture backgroundTexture1("res/textures/background1.png");
	background1.AddTexture(&backgroundTexture1);
	background1.AddShader("Background", pBackgroundShader1);


	std::shared_ptr<Shader> pBackgroundShader2 = std::make_shared<Shader>("res/shaders/DefaultBackground.shader");
	GLBackground background2;
	Texture backgroundTexture2("res/textures/background2.png");
	background2.AddTexture(&backgroundTexture2);
	background2.AddShader("Background", pBackgroundShader2);

	previewer.AddBackground(&background0);
	previewer.AddBackground(&background1);
	previewer.AddBackground(&background2);

	std::shared_ptr<Shader> pAdsShader = std::make_shared<Shader>("res/shaders/DefaultTexture.shader");
	GLMesh mesh;
	pAdsShader->Bind();
	Texture adsTexture("res/textures/texture2.png");
	mesh.AddMesh("..\\models\\plane\\plane.obj");
	mesh.AddTexture(&adsTexture);
	mesh.AddShader("Face", pAdsShader);
	previewer.AddDrawableObject(&mesh);


	//GLMesh mesh2;
	//mesh2.AddMesh("..\\models\\bunny\\bunny.obj");
	//mesh2.AddTexture(&texture);
	//mesh2.AddShader("Face", faceShader);
	//mesh2.AddShader("WireFrame", lineShader);
	//previewer.AddDrawableObject(&mesh2);
	previewer.SetBoardWidth(150);
	previewer.SetCamera(&camera);
	previewer.SetRenderer(&renderer);
	// Set max fps
	previewer.SetMaxFps(30);
	previewer.MainLoop();

    return 0;
}

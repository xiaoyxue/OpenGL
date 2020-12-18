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
#include "opengl/GLPointCloud.h"
#include "PointCloud.h"

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
		CameraType::Perspective
	);
	renderer.SetCamera(&camera);
	//model and shader

	Scene scene;
	previewer.SetPicker(&picker);
	previewer.SetScene(&scene);

	//GLMesh mesh;
	//mesh.AddMesh("..\\models\\bunny\\bunny.obj");
	//auto faceShader = std::make_shared<Shader>("res/shaders/DefaultFace.shader");
	//auto lineShader = std::make_shared<Shader>("res/shaders/DefaultLine.shader");
	////mesh2.AddTexture(&texture);
	//mesh.AddShader("Face", faceShader);
	//mesh.AddShader("WireFrame", lineShader);
	//previewer.AddDrawableObject(&mesh);

	GLPointCloud pointCloud;
	pointCloud.AddPoints("..\\models\\pointcloud\\data.txt");
	auto poinitShader = std::make_shared<Shader>("res/shaders/DefaultPoint.shader");
	previewer.AddDrawableObject(&pointCloud);
	pointCloud.AddShader("Point", poinitShader);

	previewer.SetCamera(&camera);
	previewer.SetRenderer(&renderer);
	// Set max fps
	previewer.SetMaxFps(60);
	previewer.MainLoop();

	

    return 0;
}

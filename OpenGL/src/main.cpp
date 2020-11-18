#include <iostream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Viewer.h"
#include "GLMesh.h"
#include "Previewer.h"
#include "Scene.h"


using namespace OpenGL;
using namespace Math;
using namespace GLFW;

int main(void)
{
	int resolutionX = 1280, resolutionY = 800;
	Previewer previewer("Hello World!", resolutionX, resolutionY);
	previewer.Init();

	Renderer renderer;
	Camera camera;
	camera.Init(
		Vec3(0, 0, 3),
		Vec3(0, 0, 0),
		Vec3(0, 1, 0),
		resolutionX,
		resolutionY
	);

	//model and shader
	std::shared_ptr<Shader> faceShader = std::make_shared<Shader>("res/shaders/DefaultFace.shader");
	faceShader->Bind();
	Texture texture("res/textures/texture1.png");
	texture.Bind(0);
	faceShader->SetUniform1i("u_Texture", 0);
	faceShader->UnBind();
	std::shared_ptr<Shader> lineShader = std::make_shared<Shader>("res/shaders/DefaultLine.shader");

	GLMesh mesh;
	mesh.AddMesh("..\\models\\cube\\cube.obj");
	mesh.AddTexture(&texture);
	mesh.AddShader("Face", faceShader);
	mesh.AddShader("WireFrame", lineShader);
	Scene scene;
	previewer.SetScene(&scene);
	previewer.AddDrawableObject(&mesh);
	renderer.SetCamera(&camera);
	previewer.SetRenderer(&renderer);
	previewer.MainLoop();

    return 0;
}
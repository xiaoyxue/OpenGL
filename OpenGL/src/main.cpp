#include <iostream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Camera.h"
#include "Viewer.h"
#include "GLMesh.h"
#include <limits>

using namespace OpenGL;
using namespace Math;
int main(void)
{

    Renderer renderer;
    int resolutionX = 1024, resolutionY = 768;
	Camera camera;
	camera.Init(
		Vec3(0, 0, 3),
		Vec3(0, 0, 0),
		Vec3(0, 1, 0),
		resolutionX,
		resolutionY
	);
	renderer.SetCamera(&camera);
    Viewer viewer("Hello World!");
	viewer.SetRenderer(&renderer);
    viewer.SetSize(resolutionX, resolutionY);
	viewer.Init();

	std::shared_ptr<Shader> faceShader = std::make_shared<Shader>("res/shaders/DefaultFace.shader");
	faceShader->Bind();
	Texture texture("res/textures/texture1.png");
	texture.Bind(0);
	faceShader->SetUniform1i("u_Texture", 0);
	faceShader->UnBind();

	std::shared_ptr<Shader> lineShader = std::make_shared<Shader>("res/shaders/DefaultLine.shader");
	GLMesh mesh;
	mesh.AddMesh("..\\models\\bunny\\bunny.obj");
	mesh.AddTexture(&texture);
	mesh.AddShader("Face", faceShader);
	mesh.AddShader("WireFrame", lineShader);
	viewer.Add(&mesh);
	viewer.Start();

    return 0;
}
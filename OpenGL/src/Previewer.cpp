#include "Previewer.h"
#include "Renderer.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>


namespace GLFW
{
	Previewer::Previewer(const std::string& title, int w /*= 1024*/, int h /*= 760*/)
		:GLWindow(title, w, h)
	{

	}

	Previewer::~Previewer()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Previewer::Init()
	{
		GLWindow::Init();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(mpWindow, true);
		const char* glsl_version = "#version 330";
		ImGui_ImplOpenGL3_Init(glsl_version);
	}

	void Previewer::DrawAll() const
	{
		mpRenderer->Clear();
		for (auto it : mDrawableObjects)
		{
			mpRenderer->DrawFaces(*it);
			mpRenderer->DrawWireFrame(*it);
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		{
			ImGui::Begin("Hello, world!");
			ImGui::Text("This is some useful text.");
			ImGui::End();
		}
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	}

	void Previewer::AddDrawableObject(DrawableObject* pObject)
	{
		mDrawableObjects.push_back(pObject);
	}

	bool Previewer::HandleMouseEvent()
	{
		return ImGui::GetIO().WantCaptureMouse;
	}

}


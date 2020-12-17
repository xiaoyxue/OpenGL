#include "ImmediateGui.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_utils.h"
#include "window/GLWindow.h"

namespace Gui
{

	ImmediateGui::~ImmediateGui()
	{

	}

	void ImmediateGui::InitImGui(GLWindow* pWindow) const
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(pWindow->GetGlfwWindow(), true);
		const char* glsl_version = "#version 330";
		ImGui_ImplOpenGL3_Init(glsl_version);
	}

	void ImmediateGui::ReleaseImGui() const
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	bool ImmediateGui::WantCaptureMouse() const
	{
		return ImGui::GetIO().WantCaptureMouse;
	}

}
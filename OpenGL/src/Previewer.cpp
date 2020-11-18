#include "Previewer.h"
#include "Renderer.h"
#include "Scene.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_utils.h"
#include <iostream>


namespace GLFW
{
	const ImVec2 Previewer::mButtonSize = ImVec2(200, 20);

	Previewer::Previewer(const std::string& title, int w /*= 1024*/, int h /*= 760*/)
		:GLWindow(title, w, h)
	{

	}

	Previewer::~Previewer()
	{
		ReleaseImGui();
	}

	void Previewer::Init()
	{
		GLWindow::Init();
		InitState();
		InitImGui();
	}

	void Previewer::DrawAll() const
	{
		mpRenderer->Clear();
		DrawObjects();
		if (mShowCoordnates)
			mpRenderer->DrawCoordinates();
		DrawGui();
	}

	void Previewer::SetScene(Scene* pScene)
	{
		mpScene = pScene;
	}

	void Previewer::AddDrawableObject(DrawableObject* pObject)
	{
		if(mpScene)
			mpScene->GetDrawObjects().push_back(pObject);
	}

	bool Previewer::HandleGLMouseEvent() const
	{
		return !ImGui::GetIO().WantCaptureMouse;
	}

	void Previewer::InitImGui() const
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(mpWindow, true);
		const char* glsl_version = "#version 330";
		ImGui_ImplOpenGL3_Init(glsl_version);

	}

	void Previewer::InitState()
	{
		mShowCoordnates = false;
	}

	void Previewer::ReleaseImGui() const
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Previewer::DrawGui() const
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		{
			static const float DISTANCE = 10.f;
			static int corner = 1;
			ImGuiIO& io = ImGui::GetIO();
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
			ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
			ImVec2 window_pos_pivot = ImVec2(1.f, 0.f);
			ImGui::SetNextWindowBgAlpha(0.95f); // Transparent background
			ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
			ImGui::SetNextWindowSize(ImVec2(350, io.DisplaySize.y - 2.f * DISTANCE));

			if (ImGui::Begin("Example: Simple overlay", NULL, window_flags))
			{

				ImGui::CenterText("Image Resolution: %d, %d", int(io.DisplaySize.x), int(io.DisplaySize.y));

				ImGui::Separator();
				ImGui::CenterCursor(mButtonSize.x);
				if (ImGui::Button(!mRendering ? "Render" : "Stop", mButtonSize))
				{
					mRendering = !mRendering;
				}
				ImGui::CenterCursor(mButtonSize.x);
				if (ImGui::Button("Load Model", mButtonSize))
				{

				}
				ImGui::CenterCursor(mButtonSize.x);
				if (ImGui::Button("Save Image", mButtonSize))
				{
					
				}
				ImGui::Separator();
				const char* displayModeItem[] = { "Face", "Mesh", "Edit" };
				ImGui::SetNextItemWidth(150);
				ImGui::Combo("Display Mode", (int*)(&mDisplayMode), displayModeItem, IM_ARRAYSIZE(displayModeItem));
				ImGui::Checkbox("  LockCamera", &mpRenderer->mLockCamera);
				ImGui::Checkbox("  ShowCoordinates", &mShowCoordnates);
				if (ImGui::IsMousePosValid())
					ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
				else
					ImGui::Text("Mouse Position: <invalid>");
			}
			ImGui::End();
		}
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	}

	void Previewer::DrawObjects() const
	{
		if (mDisplayMode == DispayMode::Face)
		{
			for (auto it : mpScene->GetDrawObjects())
			{
				mpRenderer->DrawFaces(*it);
				
			}
		}
		else if (mDisplayMode == DispayMode::Mesh)
		{
			for (auto it : mpScene->GetDrawObjects())
			{
				mpRenderer->DrawFaces(*it);
				mpRenderer->DrawWireFrame(*it);
			}
		}
		else if (mDisplayMode == DispayMode::Edit)
		{
			for (auto it : mpScene->GetDrawObjects())
			{
				mpRenderer->TestPick(*it);
			}
		}
	}

	void Previewer::MouseButtonCallbackFunc(int button, int action, int mods)
	{
		if (!HandleGLMouseEvent())
			return;
		if(mDisplayMode != DispayMode::Edit)
			mpRenderer->MouseButtonEvent(button, action, mods);
		else
		{
			if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
				mpRenderer->MousePick();
		}


	}

	void Previewer::ScrollCallbackFunc(double xoffset, double yoffset)
	{

	}

	void Previewer::CursorCallbackFunc(double xpos, double ypos)
	{
		mpRenderer->CursorEvent((float)xpos, (float)ypos);
	}

	void Previewer::ResizeCallbackFunc(int width, int height)
	{
		mpRenderer->Resize(mWidth, mHeight);
	}

	void Previewer::KeyCallbackFunc(int key, int scancode, int action, int mods)
	{
		mpRenderer->KeyBoardEvent(key, scancode, action, mDeltaTime);
	}

}


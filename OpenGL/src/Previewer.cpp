#include "Previewer.h"
#include "Renderer.h"
#include "Scene.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_utils.h"
#include "Camera.h"
#include "Misc.h"
#include <iostream>
#include "Picker.h"


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

	void Previewer::SetCamera(Camera* pCamera)
	{
		mpCamera = pCamera;
	}

	void Previewer::AddDrawableObject(DrawableObject* pObject)
	{
		if(mpScene)
			mpScene->AddDrawableObject(pObject);
	}

	unsigned int Previewer::Pick(int x, int y)
	{
		int id = -1;
		mpPicker->Bind();
		mpRenderer->Clear();
		DrawIdMap();
		glFlush();
		glFinish();
		id = mpPicker->Pick(x, y);
		mpPicker->UnBind();
		return id;
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
				ImGui::Checkbox("  LockCamera", &mLockCamera);
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
				mpRenderer->DrawFaces(*it);
			}
		}
	}

	void Previewer::DrawIdMap()
	{
		mpRenderer->DrawObjectsId(*mpScene);
	}

	void Previewer::MouseButtonCallbackFunc(int button, int action, int mods)
	{
		if (!HandleGLMouseEvent())
			return;
		if(mDisplayMode != DispayMode::Edit)
			MouseButtonEvent(button, action, mods);
		else
		{
			if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
			{
				Pick(mMouseX, mMouseY);
				//mpRenderer->MousePick();
			}
				
		}


	}

	void Previewer::ScrollCallbackFunc(double xoffset, double yoffset)
	{

	}

	void Previewer::CursorCallbackFunc(double xpos, double ypos)
	{
		CursorEvent((float)xpos, (float)ypos);
	}

	void Previewer::ResizeCallbackFunc(int width, int height)
	{
		mpRenderer->Resize(mWidth, mHeight);
	}

	void Previewer::KeyCallbackFunc(int key, int scancode, int action, int mods)
	{
		KeyBoardEvent(key, scancode, action, mDeltaTime);
	}

	void Previewer::MouseLeftDrag(float x, float y)
	{
		if (!mLockCamera)
		{
			float dx = x - mMouseX;
			float dy = y - mMouseY;
			float dPhi = dx * (PI / mWidth);
			float dTheta = dy * (PI / mHeight);
			mpCamera->Rotate(dPhi, -dTheta);
		}
	}

	void Previewer::MouseMiddleDrag(float x, float y)
	{
		if (!mLockCamera)
			mpCamera->Move(-(x - mMouseX) / float(mWidth), (y - mMouseY) / float(mHeight));
	}

	void Previewer::MouseLeftRightDrag(float x, float y)
	{
		if (!mLockCamera)
			mpCamera->Move(-(x - mMouseX) / float(mWidth), (y - mMouseY) / float(mHeight));
	}

	void Previewer::MouseRightDrag(float x, float y)
	{
		if (!mLockCamera)
			mpCamera->Scale((y - mMouseY) / float(mHeight));
	}

	void Previewer::MouseButtonEvent(int button, int action, int mods)
	{
		if (button == MOUSE_LEFT)
		{
			if (action == MOUSE_BUTTON_PRESS)
				mMouseLeftDown = true;
			else
				mMouseLeftDown = false;
		}
		else if (button == MOUSE_RIGHT)
		{
			if (action == MOUSE_BUTTON_PRESS)
				mMouseRightDown = true;
			else
				mMouseRightDown = false;
		}
		else if (button == MOUSE_MIDDLE)
		{
			if (action == MOUSE_BUTTON_PRESS)
				mMouseMiddleDown = true;
			else
				mMouseMiddleDown = false;
		}
	}

	void Previewer::KeyBoardEvent(int key, int event, int mods, float deltaTime)
	{
		if (!mLockCamera)
		{
			if (key == KEYBOARD_W) {
				mpCamera->MoveY(deltaTime);
			}
			else if (key == KEYBOARD_S) {
				mpCamera->MoveY(-deltaTime);
			}
			else if (key == KEYBOARD_A) {
				mpCamera->MoveX(-deltaTime);
			}
			else if (key == KEYBOARD_D) {
				mpCamera->MoveX(deltaTime);
			}
		}

	}

	void Previewer::CursorEvent(float x, float y)
	{
		if (mMouseLeftDown && !mMouseMiddleDown && !mMouseRightDown)
			MouseLeftDrag(x, y);
		else if (!mMouseLeftDown && !mMouseMiddleDown && mMouseRightDown)
			MouseRightDrag(x, y);
		else if (!mMouseLeftDown && mMouseMiddleDown && !mMouseRightDown)
			MouseMiddleDrag(x, y);
		else if (mMouseLeftDown && !mMouseMiddleDown && mMouseRightDown)
			MouseLeftRightDrag(x, y);

		mMouseX = x;
		mMouseY = y;
		mpRenderer->SetMousePoseition(mMouseX, mMouseY);
	}
}


#include "Previewer.h"
#include "opengl/Renderer.h"
#include "opengl/Scene.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_utils.h"
#include "opengl/Camera.h"
#include "opengl/Misc.h"
#include "opengl/Picker.h"
#include "opengl/Scene.h"
#include "opengl/DrawableObject.h"
#include <iostream>

using namespace Math;

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
	DrawBackgrounds();
	DrawObjects();
	if (mDisplayMode == DispayMode::CameraView)
		DrawTraceBBox();
	if (mShowCoordnates)
		mpRenderer->DrawCoordinates();
	DrawBoarder();
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
	if (mpScene)
		mpScene->AddDrawableObject(pObject);
}

void Previewer::AddBackground(DrawableObject* pBackground)
{
	if (mpScene)
		mpScene->AddBackground(pBackground);
}

int Previewer::Pick(int x, int y)
{
	int id = -1;
	mpPicker->Bind();
	mpRenderer->Clear();
	mpRenderer->EnableDepthTest();
	id = mpPicker->Pick(x, y, *mpScene, *mpRenderer);
	mpPicker->UnBind();
	mpScene->SetPickId(id);
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
	mLockCamera = false;
	mIsPicked = false;
	mMouseLeftDown = false;
	mMouseRightDown = false;
	mMouseMiddleDown = false;
	mDrawWireFrame = false;
	mRendering = false;
	mDisplayMode = DispayMode::CameraView;
	mEditMode = EditMode::Select;
	mRotateMode = RotateMode::Y;
	mTransformMode = TransformMode::Local;
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
		//ImGui::SetNextWindowSize(ImVec2(350, io.DisplaySize.y - 2.f * DISTANCE));
		ImGui::SetNextWindowSize(ImVec2(350, -1));

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
			const char* displayModeItem[] = { "CameraView", "Mesh", "Edit" };
			ImGui::SetNextItemWidth(150);
			ImGui::Combo("Display Mode", (int*)(&mDisplayMode), displayModeItem, IM_ARRAYSIZE(displayModeItem));
			ImGui::Checkbox("  LockCamera", &mLockCamera);
			ImGui::Checkbox("  LockCameraRotation", &mEnableCameraRotation);
			ImGui::Checkbox("  ShowCoordinates", &mShowCoordnates);
			if (mDisplayMode == DispayMode::Edit)
			{
				ImGui::Text("Edit mode");

				const char* transformModelItem[] = { "Local", "Global"};
				ImGui::SetNextItemWidth(150);
				ImGui::Combo("Transform mode", (int*)(&mTransformMode), transformModelItem, IM_ARRAYSIZE(transformModelItem));
				ImGui::Separator();
				ImGui::RadioButton("Translate ", (int*)&mEditMode, 1); ImGui::SameLine();
				ImGui::RadioButton("Rotate", (int*)&mEditMode, 2); ImGui::SameLine();
				ImGui::RadioButton("Scale", (int*)&mEditMode, 3); ImGui::SameLine();
				ImGui::RadioButton("Select", (int*)&mEditMode, 0);
				if(mpScene->IsSelected())
				{
					ImGui::Checkbox("TraceCamera", &(mpScene->PickObject()->GetTraceFlag()));
					{
						const bool traceFlag = mpScene->PickObject()->GetTraceFlag();
						if (traceFlag)
						{
							mTargets.insert(mpScene->PickObject());
						}
						else
						{
							mTargets.erase(mpScene->PickObject());
						}
					}
				}

				if (mEditMode == EditMode::Rotate)
				{
					ImGui::SetCursorPosX(30); ImGui::Text("Rotate model");
					ImGui::SetCursorPosX(30); ImGui::RadioButton("Rotate around Y ", (int*)&mRotateMode, 0);
					ImGui::SetCursorPosX(30); ImGui::RadioButton("Rotate around X", (int*)&mRotateMode, 1);
					ImGui::SetCursorPosX(30); ImGui::RadioButton("Rotate around XY", (int*)&mRotateMode, 2);
					ImGui::SetCursorPosX(30); ImGui::RadioButton("Rotate around Z", (int*)&mRotateMode, 3);
				}
			}
			if (ImGui::IsMousePosValid())
				ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
			else
				ImGui::Text("Mouse Position: <invalid>");
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}
		ImGui::End();
	}
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void Previewer::DrawObjects() const
{
	if (mDisplayMode == DispayMode::CameraView)
	{
		mpRenderer->DrawFaces(*mpScene);
	}
	else if (mDisplayMode == DispayMode::Mesh)
	{
		mpRenderer->DrawFaces(*mpScene);
		mpRenderer->DrawWireFrame(*mpScene);
	}
	else if (mDisplayMode == DispayMode::Edit)
	{
		mpRenderer->DrawFaces(*mpScene);
		DrawPickedBBox();
	}
}

void Previewer::DrawPickedBBox() const
{
	if (mIsPicked)
	{
		DrawableObject* pObject = mpScene->PickObject();
		mpRenderer->DrawBBox(*pObject);
	}
		
}

void Previewer::DrawTraceBBox() const
{
	for (auto it : mTargets)
	{
		mpRenderer->DrawBBox(*it, Vec4(0.0f, 1.0f, 0.0f, 0.5f));
	}
}

void Previewer::DrawBackgrounds() const
{
	auto backgrounds = mpScene->GetBackground();
	if (backgrounds.size() > 0)
	{
		auto pBackground = backgrounds[mBackgroundIndex];
		pBackground->DrawFace(*mpRenderer);
		mBackgroundIndex = (mBackgroundIndex + 1) % backgrounds.size();
	}

}

void Previewer::DrawBoarder() const
{
	mpRenderer->DrawBoarder(mBoarderWidth);
}

void Previewer::MouseButtonCallbackFunc(int button, int action, int mods)
{
	if (!HandleGLMouseEvent())
		return;
	if (mDisplayMode != DispayMode::Edit)
	{
		mIsPicked = false;
		MouseButtonEvent(button, action, mods);
	}
	else if(mDisplayMode == DispayMode::Edit)
	{
		if (mEditMode == EditMode::Select && button == MOUSE_BUTTON_LEFT && action == MOUSE_BUTTON_PRESS)
		{
			if (Pick(mMouseX, mMouseY) >= 0)
				mIsPicked = true;
			else
				mIsPicked = false;
		}
		MouseButtonEvent(button, action, mods);
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
	mWidth = width;
	mHeight = height;
	mpCamera->Resize(width, height);
	mpRenderer->Resize(width, height);
	mpPicker->Resize(width, height);
}

void Previewer::KeyCallbackFunc(int key, int scancode, int action, int mods)
{
	KeyBoardEvent(key, scancode, action, mDeltaTime);
}

void Previewer::MouseLeftDragCamera(float x, float y)
{
	if (!mLockCamera && mEnableCameraRotation)
	{
		float dx = x - mMouseX;
		float dy = y - mMouseY;
		float dPhi = dx * (PI / mWidth);
		float dTheta = dy * (PI / mHeight);
		mpCamera->Rotate(dPhi, -dTheta);
	}
}

void Previewer::MouseMiddleDragCamera(float x, float y)
{
	if (!mLockCamera)
	{
		float dx_w = (x - mMouseX) / float(mWidth);
		float dy_h = (y - mMouseY) / float(mHeight);
		mpCamera->Move(-dx_w, dy_h);
		for (auto it : mTargets)
		{
			it->mTraceMatrix = mpCamera->GetTraceMatrix() * it->mTraceMatrix;
		}
		mpCamera->UpdateTraceMatrix();
	}

}

void Previewer::MouseLeftRightDragCamera(float x, float y)
{
	if (!mLockCamera)
	{
		float dx_w = (x - mMouseX) / float(mWidth);
		float dy_h = (y - mMouseY) / float(mHeight);
		mpCamera->Move(-dx_w, dy_h);
		for (auto it : mTargets)
		{
			it->mTraceMatrix = mpCamera->GetTraceMatrix() * it->mTraceMatrix;
			//std::cout << mpCamera->GetTraceMatrix() << std::endl;
		}
		mpCamera->UpdateTraceMatrix();
	}

}

void Previewer::MouseLeftDragOject(float x, float y)
{
	if (mEditMode == EditMode::Translate)
	{
		if(mTransformMode == TransformMode::Global)
			mpScene->TranslateSelection((x - mMouseX) / mWidth , -(y - mMouseY) / mHeight, 0);
		else if (mTransformMode == TransformMode::Local)
			mpScene->TranslateSelectionLocal((x - mMouseX) / mWidth, -(y - mMouseY) / mHeight, 0);
	}
	else if (mEditMode == EditMode::Rotate)
	{
		static float rotateSpeed = 30.f;
		float dx = x - mMouseX;
		float dy = y - mMouseY;
		float dPhi = dx * (PI / mWidth) * rotateSpeed;
		float dTheta = dy * (PI / mHeight) * rotateSpeed;
		if (mTransformMode == TransformMode::Global)
		{
			if (mRotateMode == RotateMode::Y)
				mpScene->RotateSelection("Y", dPhi, 0);
			else if (mRotateMode == RotateMode::X)
				mpScene->RotateSelection("Y", 0, dTheta);
			else if (mRotateMode == RotateMode::XY)
				mpScene->RotateSelection("XY", dPhi, dTheta);
			else if (mRotateMode == RotateMode::Z)
				mpScene->RotateSelection("Z", -dPhi, 0);
		}
		else if (mTransformMode == TransformMode::Local)
		{
			if (mRotateMode == RotateMode::Y)
				mpScene->RotateSelectionLocal("Y", dPhi, 0);
			else if (mRotateMode == RotateMode::X)
				mpScene->RotateSelectionLocal("X", 0, dTheta);
			else if (mRotateMode == RotateMode::XY)
				mpScene->RotateSelectionLocal("XY", dPhi, dTheta);
			else if (mRotateMode == RotateMode::Z)
				mpScene->RotateSelectionLocal("Z", -dPhi, 0);
		}

	}
	else if (mEditMode == EditMode::Scale)
	{
		static constexpr float zoomFactor = 0.01f;
		static constexpr float zoomInFactor = 1 + zoomFactor;
		static constexpr float zoomOutFactor = 1 - zoomFactor;
		if (mTransformMode == TransformMode::Global)
			mpScene->ScaleSelection((y - mMouseY) > 0 ? zoomOutFactor : zoomInFactor);
		else if (mTransformMode == TransformMode::Local)
			mpScene->ScaleSelectionLocal((y - mMouseY) > 0 ? zoomOutFactor : zoomInFactor);
	}
}

void Previewer::MouseRightDragObject(float x, float y)
{
	if (mEditMode == EditMode::Translate)
	{
		if(mTransformMode == TransformMode::Global)
			mpScene->TranslateSelection(0, 0, (y - mMouseY) / mHeight);
		else if(mTransformMode == TransformMode::Local)
			mpScene->TranslateSelectionLocal(0, 0, (y - mMouseY) / mHeight);
	}
}

void Previewer::MouseRightDragCamera(float x, float y)
{
	if (!mLockCamera)
		mpCamera->Scale((y - mMouseY) / float(mHeight));
}

void Previewer::MouseButtonEvent(int button, int action, int mods)
{
	if (button == MOUSE_BUTTON_LEFT)
	{
		if (action == MOUSE_BUTTON_PRESS)
			mMouseLeftDown = true;
		else
			mMouseLeftDown = false;
	}
	else if (button == MOUSE_BUTTON_RIGHT)
	{
		if (action == MOUSE_BUTTON_PRESS)
			mMouseRightDown = true;
		else
			mMouseRightDown = false;
	}
	else if (button == MOUSE_BUTTON_MIDDLE)
	{
		if (action == MOUSE_BUTTON_PRESS)
			mMouseMiddleDown = true;
		else
			mMouseMiddleDown = false;
	}
}

void Previewer::KeyBoardEvent(int key, int event, int mods, float deltaTime)
{
	if (!mLockCamera && mDisplayMode != DispayMode::Edit)
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
	if (mDisplayMode != DispayMode::Edit)
	{
		if (mMouseLeftDown && !mMouseMiddleDown && !mMouseRightDown)
			MouseLeftDragCamera(x, y);
		else if (!mMouseLeftDown && !mMouseMiddleDown && mMouseRightDown)
			MouseRightDragCamera(x, y);
		else if (!mMouseLeftDown && mMouseMiddleDown && !mMouseRightDown)
			MouseMiddleDragCamera(x, y);
		else if (mMouseLeftDown && !mMouseMiddleDown && mMouseRightDown)
			MouseLeftRightDragCamera(x, y);
	}
	else if (mDisplayMode == DispayMode::Edit)
	{
		if (mEditMode != EditMode::Select && (mMouseLeftDown && !mMouseMiddleDown && !mMouseRightDown))
			MouseLeftDragOject(x, y);
		else if (mEditMode == EditMode::Translate && (!mMouseLeftDown && !mMouseMiddleDown && mMouseRightDown))
			MouseRightDragObject(x, y);
	}

	mMouseX = x;
	mMouseY = y;
}


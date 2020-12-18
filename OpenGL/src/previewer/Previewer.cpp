#include "Previewer.h"
#include "opengl/Renderer.h"
#include "opengl/Scene.h"
#include "visual/Camera.h"
#include "opengl/Misc.h"
#include "opengl/Picker.h"
#include "opengl/Scene.h"
#include "opengl/DrawableObject.h"
#include "opengl/GLPointCloud.h"
#include "opengl/GLMesh.h"
#include <iostream>



namespace Preview
{
	using namespace Math;


	Previewer::Previewer(const std::string& title, int w /*= 1024*/, int h /*= 760*/)
		:GLWindow(title, w, h)
	{
		GLWindow::Init();
		mpScene = std::make_unique<Scene>();
		mpCamera = std::make_unique<Camera>();
		mpPicker = std::make_unique<Picker>(w, h);
		mpGui = std::make_unique<PreviewerGui>(this);
		mpCamera->Init(
			Vec3(0, 0, 3),
			Vec3(0, 0, 0),
			Vec3(0, 1, 0),
			w,
			h,
			CameraType::Perspective
		);
		mpRendererOwner = std::make_unique<Renderer>(w, h);
		mpRendererOwner->SetCamera(mpCamera.get());
		SetRenderer(mpRendererOwner.get());
	}

	Previewer::~Previewer()
	{
		mpGui->ReleaseImGui();
	}

	void Previewer::Init()
	{
		//GLWindow::Init();
		InitState();
		mpGui->InitImGui(this);
	}

	void Previewer::DrawAll() const
	{
		mpRenderer->Clear();
		DrawObjects();
		if (gShowCoordnates)
			mpRenderer->DrawCoordinates();
		mpGui->Draw();
	}

	//void Previewer::SetScene(Scene* pScene)
	//{
	//	mpScene = pScene;
	//}

	//void Previewer::SetCamera(Camera* pCamera)
	//{
	//	mpCamera = pCamera;
	//	if(mpScene)
	//		mpScene->Setca
	//}

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

	void Previewer::LoadModel(const std::string& filename)
	{
		ParseInput(filename);
	}

	void Previewer::ParseInput(const std::string& filename)
	{
		int len = filename.length();
		if (filename.substr(len - 4) == ".obj")
		{
			auto pMesh = std::make_unique<GLMesh>();
			auto faceShader = std::make_shared<Shader>("res/shaders/DefaultFace.shader");
			auto lineShader = std::make_shared<Shader>("res/shaders/DefaultLine.shader");
			pMesh->AddMesh(filename);
			pMesh->AddShader("Face", faceShader);
			pMesh->AddShader("WireFrame", lineShader);
			std::unique_ptr<DrawableObject> pDrawableObject = std::move(pMesh);
			mDrawableObjects.push_back(std::move(pDrawableObject));
			AddDrawableObject(mDrawableObjects.back().get());
		}
		else if (filename.substr(len - 4) == ".txt")
		{
			auto pPointCloud = std::make_unique<GLPointCloud>();
			auto poinitShader = std::make_shared<Shader>("res/shaders/DefaultPoint.shader");
			pPointCloud->AddPoints(filename);
			pPointCloud->AddShader("Point", poinitShader);
			std::unique_ptr<DrawableObject> pDrawableObject = std::move(pPointCloud);
			mDrawableObjects.push_back(std::move(pDrawableObject));
			AddDrawableObject(mDrawableObjects.back().get());
			
		}
	}

	bool Previewer::HandleGLMouseEvent() const
	{
		return !mpGui->WantCaptureMouse();
	}


	bool Previewer::IsSelected() const
	{
		return mpScene->IsSelected();
	}

	void Previewer::InitState()
	{
		gShowCoordnates = false;
		gLockCamera = false;
		mIsPicked = false;
		gMouseLeftDown = false;
		gMouseRightDown = false;
		gMouseMiddleDown = false;
		gDrawWireFrame = false;
		gRendering = false;
		gDisplayMode = DispayMode::CameraView;
		gEditMode = EditMode::Select;
		gRotateMode = RotateMode::Y;
		gTransformMode = TransformMode::Local;
	}


	void Previewer::DrawGui() const
	{
		mpGui->Draw();
	}

	void Previewer::DrawObjects() const
	{
		if (gDisplayMode == DispayMode::CameraView)
		{
			mpRenderer->DrawFaces(*mpScene);
		}
		else if (gDisplayMode == DispayMode::Mesh)
		{
			mpRenderer->DrawFaces(*mpScene);
			mpRenderer->DrawWireFrame(*mpScene);
		}
		else if (gDisplayMode == DispayMode::Edit)
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

	void Previewer::MouseButtonCallbackFunc(int button, int action, int mods)
	{
		if (!HandleGLMouseEvent())
			return;
		if (gDisplayMode != DispayMode::Edit)
		{
			mIsPicked = false;
			MouseButtonEvent(button, action, mods);
		}
		else if(gDisplayMode == DispayMode::Edit)
		{
			if (gEditMode == EditMode::Select && button == MOUSE_BUTTON_LEFT && action == MOUSE_BUTTON_PRESS)
			{
				if (Pick(gMouseX, gMouseY) >= 0)
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
		if (!gLockCamera && gEnableCameraRotation)
		{
			float dx = x - gMouseX;
			float dy = y - gMouseY;
			float dPhi = dx * (PI / mWidth);
			float dTheta = dy * (PI / mHeight);
			mpCamera->Rotate(dPhi, -dTheta);
		}
	}

	void Previewer::MouseMiddleDragCamera(float x, float y)
	{
		if (!gLockCamera)
		{
			float dx_w = (x - gMouseX) / float(mWidth);
			float dy_h = (y - gMouseY) / float(mHeight);
			mpCamera->Move(-dx_w, dy_h);
		}

	}

	void Previewer::MouseLeftRightDragCamera(float x, float y)
	{
		if (!gLockCamera)
		{
			float dx_w = (x - gMouseX) / float(mWidth);
			float dy_h = (y - gMouseY) / float(mHeight);
			mpCamera->Move(-dx_w, dy_h);
		}

	}

	void Previewer::MouseLeftDragOject(float x, float y)
	{
		if (gEditMode == EditMode::Translate)
		{
			if(gTransformMode == TransformMode::Global)
				mpScene->TranslateSelection((x - gMouseX) / mWidth , -(y - gMouseY) / mHeight, 0);
			else if (gTransformMode == TransformMode::Local)
				mpScene->TranslateSelectionLocal((x - gMouseX) / mWidth, -(y - gMouseY) / mHeight, 0);
		}
		else if (gEditMode == EditMode::Rotate)
		{
			static float rotateSpeed = 30.f;
			float dx = x - gMouseX;
			float dy = y - gMouseY;
			float dPhi = dx * (PI / mWidth) * rotateSpeed;
			float dTheta = dy * (PI / mHeight) * rotateSpeed;
			if (gTransformMode == TransformMode::Global)
			{
				if (gRotateMode == RotateMode::Y)
					mpScene->RotateSelection("Y", dPhi, 0);
				else if (gRotateMode == RotateMode::X)
					mpScene->RotateSelection("Y", 0, dTheta);
				else if (gRotateMode == RotateMode::XY)
					mpScene->RotateSelection("XY", dPhi, dTheta);
				else if (gRotateMode == RotateMode::Z)
					mpScene->RotateSelection("Z", -dPhi, 0);
			}
			else if (gTransformMode == TransformMode::Local)
			{
				if (gRotateMode == RotateMode::Y)
					mpScene->RotateSelectionLocal("Y", dPhi, 0);
				else if (gRotateMode == RotateMode::X)
					mpScene->RotateSelectionLocal("X", 0, dTheta);
				else if (gRotateMode == RotateMode::XY)
					mpScene->RotateSelectionLocal("XY", dPhi, dTheta);
				else if (gRotateMode == RotateMode::Z)
					mpScene->RotateSelectionLocal("Z", -dPhi, 0);
			}

		}
		else if (gEditMode == EditMode::Scale)
		{
			static constexpr float zoomFactor = 0.01f;
			static constexpr float zoomInFactor = 1 + zoomFactor;
			static constexpr float zoomOutFactor = 1 - zoomFactor;
			if (gTransformMode == TransformMode::Global)
				mpScene->ScaleSelection((y - gMouseY) > 0 ? zoomOutFactor : zoomInFactor);
			else if (gTransformMode == TransformMode::Local)
				mpScene->ScaleSelectionLocal((y - gMouseY) > 0 ? zoomOutFactor : zoomInFactor);
		}
	}

	void Previewer::MouseRightDragObject(float x, float y)
	{
		if (gEditMode == EditMode::Translate)
		{
			if(gTransformMode == TransformMode::Global)
				mpScene->TranslateSelection(0, 0, (y - gMouseY) / mHeight);
			else if(gTransformMode == TransformMode::Local)
				mpScene->TranslateSelectionLocal(0, 0, (y - gMouseY) / mHeight);
		}
	}

	void Previewer::MouseRightDragCamera(float x, float y)
	{
		if (!gLockCamera)
			mpCamera->Scale((y - gMouseY) / float(mHeight));
	}

	void Previewer::MouseButtonEvent(int button, int action, int mods)
	{
		if (button == MOUSE_BUTTON_LEFT)
		{
			if (action == MOUSE_BUTTON_PRESS)
				gMouseLeftDown = true;
			else
				gMouseLeftDown = false;
		}
		else if (button == MOUSE_BUTTON_RIGHT)
		{
			if (action == MOUSE_BUTTON_PRESS)
				gMouseRightDown = true;
			else
				gMouseRightDown = false;
		}
		else if (button == MOUSE_BUTTON_MIDDLE)
		{
			if (action == MOUSE_BUTTON_PRESS)
				gMouseMiddleDown = true;
			else
				gMouseMiddleDown = false;
		}
	}

	void Previewer::KeyBoardEvent(int key, int event, int mods, float deltaTime)
	{
		if (!gLockCamera && gDisplayMode != DispayMode::Edit)
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
		if (gDisplayMode != DispayMode::Edit)
		{
			if (gMouseLeftDown && !gMouseMiddleDown && !gMouseRightDown)
				MouseLeftDragCamera(x, y);
			else if (!gMouseLeftDown && !gMouseMiddleDown && gMouseRightDown)
				MouseRightDragCamera(x, y);
			else if (!gMouseLeftDown && gMouseMiddleDown && !gMouseRightDown)
				MouseMiddleDragCamera(x, y);
			else if (gMouseLeftDown && !gMouseMiddleDown && gMouseRightDown)
				MouseLeftRightDragCamera(x, y);
		}
		else if (gDisplayMode == DispayMode::Edit)
		{
			if (gEditMode != EditMode::Select && (gMouseLeftDown && !gMouseMiddleDown && !gMouseRightDown))
				MouseLeftDragOject(x, y);
			else if (gEditMode == EditMode::Translate && (!gMouseLeftDown && !gMouseMiddleDown && gMouseRightDown))
				MouseRightDragObject(x, y);
		}

		gMouseX = x;
		gMouseY = y;
	}
}




#include "Previewer.h"
#include "opengl/Renderer.h"
#include "opengl/Scene.h"
#include "visual/Camera.h"
#include "opengl/Misc.h"
#include "opengl/Picker.h"
#include "opengl/Scene.h"
#include "opengl/DrawableObject.h"
#include "opengl/DrawQuad.h"
#include "Preview.h"
#include <iostream>

std::vector<unsigned char> imageBuffer(2560 * 1440 * 3);


namespace Preview
{
	using namespace Math;
	using namespace Visual;

	Previewer::Previewer()
	{
		mpGui = std::make_unique<PreviewerGui>(this);
		//mpSSAO = std::make_unique<SSAO>();
	}

	Previewer::Previewer(const std::string& title, int w /*= 1024*/, int h /*= 760*/)
		:GLWindow(title, w, h)
	{
		mpGui = std::make_unique<PreviewerGui>(this);
	}

	Previewer::~Previewer()
	{
		mpGui->ReleaseImGui();
	}

	void Previewer::Init()
	{
		GLWindow::Init();
		InitState();
		mpGui->InitImGui(this);
		mpSSAO = std::make_unique<SSAO>(mWidth, mHeight);
	}

	void Previewer::DrawAll() const
	{
		
		if (mDrawQuad == true) {
			mpRenderer->Clear();
			mpRenderer->DrawFaces(*mpQuad);
			glReadPixels(0, 0, mWidth, mHeight, GL_RGB, GL_UNSIGNED_BYTE, &imageBuffer[0]);
			stbi_write_png("c:/Misc/ImageFrame/1.png", mWidth, mHeight, 3, &imageBuffer[0], 3 * mWidth);
			mpGui->Draw();
		}
		else {
			gUseGbuffer = true;
			if (!gUseGbuffer)
			{
				mpRenderer->Clear();
				DrawObjects();
				if (gShowCoordnates)
					mpRenderer->DrawCoordinates();
				mpGui->Draw();
			}
			else
			{
				//Draw Geometory Buffer
				mpSSAO->BindGbuffer();
				mpRenderer->Clear();
				mpRenderer->DrawGbuffer(*mpScene, mpSSAO->mFrameBuffer);
				mpSSAO->UnBindGbuffer();

				//Draw SSAO Map
				mpSSAO->BindSSAO_Buffer();
				mpRenderer->Clear();
				mpQuad->DrawSSAO(*mpRenderer, *mpSSAO);
				mpSSAO->UnBindSSAO_Buffer();



				mFrameBuffers["offlineFrameBuffer"]->Bind();
				mpRenderer->Clear();
				DrawObjects();
				mFrameBuffers["offlineFrameBuffer"]->UnBind();
				//if (gShowCoordnates)
				//	mpRenderer->DrawCoordinates();
				//mpGui->Draw();

				DebugDraw();
				if (gShowCoordnates)
					mpRenderer->DrawCoordinates();
				mpGui->Draw();
			}
		}
		
	}

	void Previewer::DebugDraw() const
	{
		mpDebugQuad->SetTexture("ssao", mpSSAO->mpSSAO_Color.get());
		mpDebugQuad->SetTexture("offlineTexture", mTextures["offlineTexture"]);

		mpRenderer->Clear();
		mpRenderer->DebugDraw(*mpDebugQuad);
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

	void Previewer::SetDebugQuad(DrawQuad* pDebugQuad)
	{
		mpDebugQuad = pDebugQuad;
	}

	void Previewer::SetQuad(DrawQuad* pQuad)
	{
		mpQuad = pQuad;
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

	void Previewer::SetSSAO()
	{
		mpRenderer->SetSSAO(mpSSAO.get());
	}

	void Previewer::SetFrameBuffer(const std::string& bufferName, FrameBuffer* pFrameBuffer)
	{
		mFrameBuffers[bufferName] = pFrameBuffer;
	}

	void Previewer::SetTexture(const std::string& textureName, Texture2D* pTexture)
	{
		mTextures[textureName] = pTexture;
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




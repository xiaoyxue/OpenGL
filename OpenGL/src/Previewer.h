#pragma once
#include "ForwardDecl.h"
#include "window/GLWindow.h"
#include "imgui/imgui.h"
#include "opengl/Picker.h"
#include <vector>
#include <set>

using namespace OpenGL;
using namespace GLFW;

namespace OpenGL {
	class DrawableObject;
}

class Previewer : public GLWindow
{
	enum class DispayMode
	{
		CameraView = 0,
		Mesh = 1,
		Edit = 2
	};

	enum class EditMode
	{
		Select = 0,
		Translate = 1,
		Rotate = 2,
		Scale = 3
	};

	enum class RotateMode
	{
		Y = 0,
		X = 1,
		XY = 2,
		Z = 3
	};

	enum class TransformMode
	{
		Local = 0,
		Global = 1
	};

private:
	Scene* mpScene;
	Camera* mpCamera;
	Picker* mpPicker;
	int mBoarderWidth;
public:
	Previewer() = default;
	Previewer(const std::string& title, int w = 1024, int h = 768);
	virtual ~Previewer();

	void Init();
	void DrawAll() const override;
	void SetScene(Scene* pScene);
	void SetCamera(Camera* pCamera);
	void SetPicker(Picker* pPicker) { mpPicker = pPicker; }
	void SetBoardWidth(int w) { mBoarderWidth = w; }
	void AddDrawableObject(DrawableObject* pObject);
	void AddBackground(DrawableObject* pBackground);

private:
	static const ImVec2 mButtonSize;
	mutable bool mRendering;
	mutable bool mShowCoordnates;
	mutable bool mLockCamera;
	mutable bool mEnableCameraRotation;
	mutable bool mMouseLeftDown;
	mutable bool mMouseRightDown;
	mutable bool mMouseMiddleDown;
	mutable bool mDrawWireFrame;
	mutable float mMouseX, mMouseY;
	mutable DispayMode mDisplayMode;
	mutable EditMode mEditMode;
	mutable RotateMode mRotateMode;
	mutable TransformMode mTransformMode;
	bool mIsPicked;
	mutable int mBackgroundIndex = 0;

	mutable std::set<DrawableObject*> mTargets;
private:
	void InitImGui() const;
	void InitState();
	void ReleaseImGui()const;
	void DrawGui() const;
	void DrawObjects() const;
	void DrawPickedBBox() const;
	void DrawTraceBBox() const;
	void DrawBackgrounds() const;
	void DrawBoarder() const;
	int Pick(int x, int y);
	bool HandleGLMouseEvent() const;


protected:
	void MouseButtonCallbackFunc(int button, int action, int mods) override;
	void ScrollCallbackFunc(double xoffset, double yoffset) override;
	void CursorCallbackFunc(double xpos, double ypos) override;
	void ResizeCallbackFunc(int width, int height) override;
	void KeyCallbackFunc(int key, int scancode, int action, int mods) override;

private:
	void MouseButtonEvent(int button, int action, int mods);
	void KeyBoardEvent(int key, int event, int mods, float deltaTime);
	void CursorEvent(float x, float y);

	void MouseRightDragCamera(float x, float y);
	void MouseLeftDragCamera(float x, float y);
	void MouseMiddleDragCamera(float x, float y);
	void MouseLeftRightDragCamera(float x, float y);


	void MouseLeftDragOject(float x, float y);
	void MouseRightDragObject(float x, float y);
};

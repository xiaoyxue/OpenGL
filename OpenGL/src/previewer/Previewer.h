#pragma once
#include "Preview.h"
#include "window/GLWindow.h"
#include "opengl/Picker.h"
#include "PreviewerGui.h"
#include "ForwardDecl.h"
#include <vector>
#include <set>


namespace OpenGL {
	class DrawableObject;
}

namespace Preview
{

	using namespace GLFW;

	class Previewer final : public GLWindow
	{
	private:
		Scene* mpScene;
		Camera* mpCamera;
		Picker* mpPicker;
		std::unique_ptr<ImmediateGui> mpGui;
		int mBoarderWidth;
	public:
		Previewer();
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

		bool HandleGLMouseEvent() const;
		bool IsSelected() const;

	private:
		bool mIsPicked;
		mutable int mBackgroundIndex = 0;
		mutable std::set<DrawableObject*> mTargets;

	private:
		// Private methods
		void InitState();
		void DrawGui() const;
		void DrawObjects() const;
		void DrawPickedBBox() const;
		void DrawTraceBBox() const;
		void DrawBackgrounds() const;
		void DrawBoarder() const;
		int Pick(int x, int y);



	protected:
		// Callback
		void MouseButtonCallbackFunc(int button, int action, int mods) override;
		void ScrollCallbackFunc(double xoffset, double yoffset) override;
		void CursorCallbackFunc(double xpos, double ypos) override;
		void ResizeCallbackFunc(int width, int height) override;
		void KeyCallbackFunc(int key, int scancode, int action, int mods) override;

	private:
		// Event
		void MouseButtonEvent(int button, int action, int mods);
		void KeyBoardEvent(int key, int event, int mods, float deltaTime);
		void CursorEvent(float x, float y);

		// Action
		void MouseRightDragCamera(float x, float y);
		void MouseLeftDragCamera(float x, float y);
		void MouseMiddleDragCamera(float x, float y);
		void MouseLeftRightDragCamera(float x, float y);

		void MouseLeftDragOject(float x, float y);
		void MouseRightDragObject(float x, float y);

	};

}


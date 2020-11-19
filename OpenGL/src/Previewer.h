#pragma once
#include "ForwardDecl.h"
#include "window/GLWindow.h"
#include "imgui/imgui.h"
#include "Picker.h"
#include <vector>


namespace GLFW
{
	using namespace OpenGL;

	class Previewer : public GLWindow
	{
		enum DispayMode
		{
			Face = 0,
			Mesh = 1,
			Edit = 2
		};
	private:
		Scene* mpScene;
		Camera* mpCamera;
		Picker* mpPicker;
	public:
		Previewer() = default;
		Previewer(const std::string& title, int w = 1024, int h = 768);
		virtual ~Previewer();

		void Init();
		void DrawAll() const override;
		void SetScene(Scene* pScene);
		void SetCamera(Camera* pCamera);
		void SetPicker(Picker* pPicker) { mpPicker = pPicker; }
		void AddDrawableObject(DrawableObject* pObject);
		unsigned int Pick(int x, int y);
		

	private:
		static const ImVec2 mButtonSize;
		mutable DispayMode mDisplayMode;
		mutable bool mRendering;
		mutable bool mShowCoordnates;
		mutable bool mLockCamera;
		mutable bool mMouseLeftDown;
		mutable bool mMouseRightDown;
		mutable bool mMouseMiddleDown;
		mutable bool mDrawWireFrame;
		mutable float mMouseX, mMouseY;
		
	private:
		void InitImGui() const;
		void InitState();
		void ReleaseImGui()const;
		void DrawGui() const;
		void DrawObjects() const;
		void DrawIdMap();
		bool HandleGLMouseEvent() const;

	protected:
		void MouseButtonCallbackFunc(int button, int action, int mods) override;
		void ScrollCallbackFunc(double xoffset, double yoffset) override;
		void CursorCallbackFunc(double xpos, double ypos) override;
		void ResizeCallbackFunc(int width, int height) override;
		void KeyCallbackFunc(int key, int scancode, int action, int mods) override;

	private:
		void MouseRightDrag(float x, float y);
		void MouseLeftDrag(float x, float y);
		void MouseMiddleDrag(float x, float y);
		void MouseLeftRightDrag(float x, float y);

		void MouseButtonEvent(int button, int action, int mods);
		void KeyBoardEvent(int key, int event, int mods, float deltaTime);
		void CursorEvent(float x, float y);
	};
}


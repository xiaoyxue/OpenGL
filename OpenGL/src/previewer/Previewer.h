#pragma once
#include "Preview.h"
#include "window/GLWindow.h"
#include "opengl/Picker.h"
#include "PreviewerGui.h"
#include "ForwardDecl.h"
#include "ssao/SSAO.h"
#include <vector>
#include <set>


namespace OpenGL {
	class DrawableObject;
}

namespace Preview
{

	using namespace GLFW;
	using namespace Visual;

	class Previewer final : public GLWindow
	{
	private:
		Scene* mpScene;
		Camera* mpCamera;
		Picker* mpPicker;
		std::unique_ptr<ImmediateGui> mpGui;
		int mBoarderWidth;
		std::unique_ptr<SSAO> mpSSAO;
		FrameBuffer* mpFrameBuffer;

		mutable std::unordered_map<std::string, Texture2D*> mTextures;
		mutable std::unordered_map<std::string, FrameBuffer*> mFrameBuffers;
	public:
		Previewer();
		Previewer(const std::string& title, int w = 1024, int h = 768);
		virtual ~Previewer();

		void Init();
		void DrawAll() const override;
		void DebugDraw() const override;
		void SetScene(Scene* pScene);
		void SetCamera(Camera* pCamera);
		void SetPicker(Picker* pPicker) { mpPicker = pPicker; }
		void SetBoardWidth(int w) { mBoarderWidth = w; }
		void AddDrawableObject(DrawableObject* pObject);
		void AddBackground(DrawableObject* pBackground);
		void SetDebugQuad(DrawQuad* pDebugQuad);
		void SetQuad(DrawQuad* pQuad);
		void SetFrameBuffer(FrameBuffer* pFrameBuffer) { mpFrameBuffer = pFrameBuffer; }
		void SetSSAO();
		void SetFrameBuffer(const std::string& bufferName, FrameBuffer* pFrameBuffer);
		void SetTexture(const std::string& textureName, Texture2D* pTexture);
		bool HandleGLMouseEvent() const;
		bool IsSelected() const;
		bool mDrawQuad = false;

	private:
		bool mIsPicked;
		mutable int mBackgroundIndex = 0;
		mutable std::set<DrawableObject*> mTargets;

		mutable DrawQuad* mpDebugQuad;
		mutable DrawQuad* mpQuad;
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


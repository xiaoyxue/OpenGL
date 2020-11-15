#pragma once
#include "Window.h"
#include "../ForwardDecl.h"


namespace GLFW
{
	using namespace OpenGL;

	class GLWindow : public Window
	{
	protected:
		GLFWwindow* mpWindow;
		Renderer* mpRenderer;
		float mDeltaTime, mLastFrame;
	public:
		GLWindow() = default;
		GLWindow(const std::string& title, int w = 1024, int h = 760);
		virtual ~GLWindow();

		void Init();
		void MainLoop();
		void SetRenderer(Renderer* pRenderer);
		virtual void DrawAll() const;

	protected:
		virtual bool HandleGLMouseEvent() const { return true; }

	private:
		void SetErrorCallback();
		void SetKeyCallback();
		void SetResizeCallback();
		void SetCursorCallback();
		void SetScrollCallback();
		void SetMouseButtonCallback();

	};
}
#pragma once
#include "ForwardDecl.h"
#include "window/GLWindow.h"
#include "imgui/imgui.h"
#include <vector>


namespace GLFW
{
	using namespace OpenGL;

	class Previewer : public GLWindow
	{
		enum DispayMode
		{
			Face = 0,
			Mesh = 1
		};
	private:
		//std::vector<DrawableObject*> mDrawableObjects;
		Scene* mpScene;
	public:
		Previewer() = default;
		Previewer(const std::string& title, int w = 1024, int h = 768);
		virtual ~Previewer();

		void Init();
		void DrawAll() const override;
		void SetScene(Scene* pScene);
		void AddDrawableObject(DrawableObject* pObject);

	protected:
		bool HandleGLMouseEvent() const override;

	private:
		static const ImVec2 mButtonSize;
		mutable DispayMode mDisplayMode;
		mutable bool mRendering;
		mutable bool mShowCoordnates;
		
	private:
		void InitImGui() const;
		void InitState();
		void ReleaseImGui()const;
		void DrawGui() const;
		void DrawObjects() const;

	};
}


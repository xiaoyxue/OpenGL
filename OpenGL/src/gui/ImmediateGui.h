#pragma once
#include "ForwardDecl.h"

namespace Gui
{
	using namespace GLFW;
	class ImmediateGui
	{
	public:
		virtual ~ImmediateGui();
		virtual void Draw() const = 0;
		void InitImGui(GLWindow* pWindow) const;
		void ReleaseImGui()const;
		bool WantCaptureMouse() const;
	};
}

#pragma once
#include "ForwardDecl.h"
#include "gui/ImmediateGui.h"
#include "math/Lingal.h"
#include "imgui/imgui.h"

bool OpenDialog(std::string& filename);

namespace Preview
{	
	using namespace Gui;
	using namespace GLFW;
	using namespace Math;
	class Previewer;
	
	class PreviewerGui : public ImmediateGui
	{
	private:
		ImVec2 mButtonSize = ImVec2(200, 20);
		Previewer* mpPreviewer;
	public:
		PreviewerGui(Previewer* pPreviewer);
		void Draw() const;

	};
}

#include "imgui_utils.h"
#include "imgui.h"
#include <cstdio>

namespace ImGui
{
	char tempBuf[10 * 1024 + 1];

	void CenterText(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		vsprintf(tempBuf, fmt, args);
		va_end(args);
		ImVec2 textSize = ImGui::CalcTextSize(tempBuf);
		ImGui::SetCursorPosX((ImGui::GetWindowWidth() - textSize.x) * 0.5f);
		ImGui::Text(tempBuf);
	}

	void CenterCursor(float width)
	{
		ImGui::SetCursorPosX((ImGui::GetWindowWidth() - width) * 0.5f);
	}
}

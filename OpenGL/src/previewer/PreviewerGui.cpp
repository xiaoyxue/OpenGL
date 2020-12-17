#include "PreviewerGui.h"
#include "Previewer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_utils.h"



namespace Preview
{

	PreviewerGui::PreviewerGui(Previewer* pPreviewer)
	{
		mpPreviewer = pPreviewer;
	}

	void PreviewerGui::Draw() const
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		{
			static const float DISTANCE = 10.f;
			static int corner = 1;
			ImGuiIO& io = ImGui::GetIO();
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
			ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
			ImVec2 window_pos_pivot = ImVec2(1.f, 0.f);
			ImGui::SetNextWindowBgAlpha(0.95f); // Transparent background
			ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
			//ImGui::SetNextWindowSize(ImVec2(350, io.DisplaySize.y - 2.f * DISTANCE));
			ImGui::SetNextWindowSize(ImVec2(350, -1));

			if (ImGui::Begin("Example: Simple overlay", NULL, window_flags))
			{

				ImGui::CenterText("Image Resolution: %d, %d", int(io.DisplaySize.x), int(io.DisplaySize.y));

				ImGui::Separator();
				ImGui::CenterCursor(mButtonSize.x);
				if (ImGui::Button(!gRendering ? "Render" : "Stop", mButtonSize))
				{
					gRendering = !gRendering;
				}
				ImGui::CenterCursor(mButtonSize.x);
				if (ImGui::Button("Load Model", mButtonSize))
				{

				}
				ImGui::CenterCursor(mButtonSize.x);
				if (ImGui::Button("Save Image", mButtonSize))
				{

				}
				ImGui::Separator();
				const char* displayModeItem[] = { "CameraView", "Mesh", "Edit" };
				ImGui::SetNextItemWidth(150);
				ImGui::Combo("Display Mode", (int*)(&gDisplayMode), displayModeItem, IM_ARRAYSIZE(displayModeItem));
				ImGui::Checkbox("  LockCamera", &gLockCamera);
				ImGui::Checkbox("  EnableCameraRotation", &gEnableCameraRotation);
				ImGui::Checkbox("  ShowCoordinates", &gShowCoordnates);
				if (gDisplayMode == DispayMode::Edit)
				{
					ImGui::Text("Edit mode");

					const char* transformModelItem[] = { "Local", "Global" };
					ImGui::SetNextItemWidth(150);
					ImGui::Combo("Transform mode", (int*)(&gTransformMode), transformModelItem, IM_ARRAYSIZE(transformModelItem));
					ImGui::Separator();
					ImGui::RadioButton("Translate ", (int*)&gEditMode, 1); ImGui::SameLine();
					ImGui::RadioButton("Rotate", (int*)&gEditMode, 2); ImGui::SameLine();
					ImGui::RadioButton("Scale", (int*)&gEditMode, 3); ImGui::SameLine();
					ImGui::RadioButton("Select", (int*)&gEditMode, 0);

					if (gEditMode == EditMode::Rotate)
					{
						ImGui::SetCursorPosX(30); ImGui::Text("Rotate model");
						ImGui::SetCursorPosX(30); ImGui::RadioButton("Rotate around Y ", (int*)&gRotateMode, 0);
						ImGui::SetCursorPosX(30); ImGui::RadioButton("Rotate around X", (int*)&gRotateMode, 1);
						ImGui::SetCursorPosX(30); ImGui::RadioButton("Rotate around XY", (int*)&gRotateMode, 2);
						ImGui::SetCursorPosX(30); ImGui::RadioButton("Rotate around Z", (int*)&gRotateMode, 3);
					}
				}
				if (ImGui::IsMousePosValid())
					ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
				else
					ImGui::Text("Mouse Position: <invalid>");
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}
			ImGui::End();
		}
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

}

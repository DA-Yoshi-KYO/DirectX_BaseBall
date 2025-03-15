#include "ImGuiManager.h"
#include "Defines.h"

void SpriteParamDebug(DirectX::XMFLOAT2* pos, DirectX::XMFLOAT2* size, std::string name)
{
	ImGui::SetMouseCursor(ImGui::GetMouseCursor());
	ImGui::SetNextWindowPos(ImVec2(20, 20));
	ImGui::SetNextWindowSize(ImVec2(200, 300));
	ImGui::Begin(name.c_str());
	ImGui::SliderFloat("XPos", &pos->x, -SCREEN_WIDTH / 2.0f, SCREEN_WIDTH / 2.0f);
	ImGui::SliderFloat("YPos", &pos->y, -SCREEN_HEIGHT / 2.0f, SCREEN_HEIGHT / 2.0f);
	ImGui::SliderFloat("XSize", &size->x, 0.0f, 1920.0f);
	ImGui::SliderFloat("YSize", &size->y, 0.0f, 1080.0f);
	ImGui::Button("Button");

	ImGui::End();
}

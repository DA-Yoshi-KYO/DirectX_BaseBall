/*+===================================================================
	File: ImguiManager.cpp
	Summary: ImGuiの簡略化
	Author: 吉田京志郎
	Date:	2025/03/15	初回作成
						floatスライダー実装
			2025/03/17	ボタンでの数値調整実装
						ボタンでのtrue,falseのスワップ実装
===================================================================+*/

// ==============================
//    インクルード部
// ==============================
#include "ImGuiManager.h"
#include "Defines.h"

void SpriteParamDebug(DirectX::XMFLOAT2* pos, DirectX::XMFLOAT2* size, std::string name,DirectX::XMFLOAT2 guipos)
{
	// 描画前の設定
	// ウィンドウを出す位置
	ImGui::SetNextWindowPos(ImVec2(guipos.x, guipos.y));
	// ウィンドウのサイズ
	ImGui::SetNextWindowSize(ImVec2(200, 150));
	// ウィンドウの名前
	ImGui::Begin(name.c_str());
	
	// コンポーネント
	// スライダー呼び出し(名前,調整したい値,min,max)
	ImGui::SliderFloat("XPos", &pos->x, -SCREEN_WIDTH / 2.0f, SCREEN_WIDTH / 2.0f);
	ImGui::SliderFloat("YPos", &pos->y, -SCREEN_HEIGHT / 2.0f, SCREEN_HEIGHT / 2.0f);
	ImGui::SliderFloat("XSize", &size->x, 0.0f, 1920.0f);
	ImGui::SliderFloat("YSize", &size->y, 0.0f, 1080.0f);
	
	// 描画後の処理
	ImGui::End();
}

void ValueDebug(int* value, std::string name, DirectX::XMFLOAT2 guipos)
{
	// 描画前の設定
	// ウィンドウを出す位置
	ImGui::SetNextWindowPos(ImVec2(guipos.x, guipos.y));
	// ウィンドウのサイズ
	ImGui::SetNextWindowSize(ImVec2(200, 150));
	// ウィンドウの名前
	ImGui::Begin(name.c_str());
	
	// コンポーネント
	// ボタン呼び出し
	if (ImGui::Button("Add")) (*value)++;
	if (ImGui::Button("Minus")) (*value)--;
	if (ImGui::Button("Add10")) (*value) += 10;
	if (ImGui::Button("Minus10")) (*value) -= 10;

	// 描画後の処理
	ImGui::End();
}

void BoolDebug(bool* value, std::string name, DirectX::XMFLOAT2 guipos)
{

	// 描画前の設定
	// ウィンドウを出す位置
	ImGui::SetNextWindowPos(ImVec2(guipos.x, guipos.y));
	// ウィンドウのサイズ
	ImGui::SetNextWindowSize(ImVec2(200, 150));
	// ウィンドウの名前
	ImGui::Begin(name.c_str());

	// コンポーネント
	// ボタン呼び出し
	if (ImGui::Button("Change"))(*value) ^= true;

	// 描画後の処理
	ImGui::End();
}

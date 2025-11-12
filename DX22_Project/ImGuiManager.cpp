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

void ModelParamDebug(DirectX::XMFLOAT3* pos, DirectX::XMFLOAT3* size, DirectX::XMFLOAT3* rotate, std::string name, DirectX::XMFLOAT2 guipos)
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
	ImGui::SliderFloat("XPos", &pos->x, -SCREEN_WIDTH  / 2.0f + WORLD_AJUST, SCREEN_WIDTH / 2.0f + WORLD_AJUST);
	ImGui::SliderFloat("YPos", &pos->y, -SCREEN_HEIGHT / 2.0f + WORLD_AJUST, SCREEN_HEIGHT / 2.0f + WORLD_AJUST);
	ImGui::SliderFloat("ZPos", &pos->y, -SCREEN_HEIGHT / 2.0f + WORLD_AJUST, SCREEN_HEIGHT / 2.0f + WORLD_AJUST);
	ImGui::SliderFloat("XSize", &size->x, 0.0f, 400.0f);
	ImGui::SliderFloat("YSize", &size->y, 0.0f, 400.0f);
	ImGui::SliderFloat("ZSize", &size->z, 0.0f, 400.0f);
	ImGui::SliderFloat("XRotate", &rotate->x, 0.0f, 360.0f);
	ImGui::SliderFloat("YRotate", &rotate->y, 0.0f, 360.0f);
	ImGui::SliderFloat("ZRotate", &rotate->z, 0.0f, 360.0f);
	rotate->x = DirectX::XMConvertToRadians(rotate->x);
	rotate->y = DirectX::XMConvertToRadians(rotate->y);
	rotate->z = DirectX::XMConvertToRadians(rotate->z);

	if (ImGui::Button("Reset"))
	{
		pos->x = 0.0f;
		pos->y = 0.0f;
		pos->z = 0.0f;
		size->x = 0.0f;
		size->y = 0.0f;
		size->z = 0.0f;
		rotate->x = 0.0f;
		rotate->y = 0.0f;
		rotate->z = 0.0f;
	}

	// 描画後の処理
	ImGui::End();
}

DirectX::XMFLOAT4 GetPosSizeDebug(std::string name, DirectX::XMFLOAT2 guipos)
{
	static DirectX::XMFLOAT4 PosSize = { 0.0f,0.0f,100.0f,100.0f };

	// 描画前の設定
	// ウィンドウを出す位置
	ImGui::SetNextWindowPos(ImVec2(guipos.x, guipos.y));
	// ウィンドウのサイズ
	ImGui::SetNextWindowSize(ImVec2(200, 150));
	// ウィンドウの名前
	ImGui::Begin(name.c_str());

	// コンポーネント
	// スライダー呼び出し(名前,調整したい値,min,max)
	ImGui::SliderFloat("XPos", &PosSize.x, -SCREEN_WIDTH / 2.0f, SCREEN_WIDTH / 2.0f);
	ImGui::SliderFloat("YPos", &PosSize.y, -SCREEN_HEIGHT / 2.0f, SCREEN_HEIGHT / 2.0f);
	ImGui::SliderFloat("XSize", &PosSize.z, 0.0f, 1920.0f);
	ImGui::SliderFloat("YSize", &PosSize.w, 0.0f, 1080.0f);

	// 描画後の処理
	ImGui::End();

	return PosSize;
}

DirectX::XMFLOAT3X3 GetPosSizeRotateDebug(std::string name, DirectX::XMFLOAT2 guipos)
{
	static DirectX::XMFLOAT3X3 PosSizeRotate = {};

	// 描画前の設定
	// ウィンドウを出す位置
	ImGui::SetNextWindowPos(ImVec2(guipos.x, guipos.y));
	// ウィンドウのサイズ
	ImGui::SetNextWindowSize(ImVec2(200, 150));
	// ウィンドウの名前
	ImGui::Begin(name.c_str());

	// コンポーネント
	// スライダー呼び出し(名前,調整したい値,min,max)
	ImGui::SliderFloat("XPos", &PosSizeRotate._11, -SCREEN_WIDTH, 150.0f);
	ImGui::SliderFloat("YPos", &PosSizeRotate._12, -SCREEN_WIDTH, 150.0f);
	ImGui::SliderFloat("ZPos", &PosSizeRotate._13, -SCREEN_WIDTH, 150.0f);
	ImGui::SliderFloat("XSize", &PosSizeRotate._21, 0.0f, 20.0f);
	ImGui::SliderFloat("YSize", &PosSizeRotate._22, 0.0f, 20.0f);
	ImGui::SliderFloat("ZSize", &PosSizeRotate._23, 0.0f, 20.0f);
	ImGui::SliderFloat("XRotate", &PosSizeRotate._31,  0.0f,DirectX::XMConvertToRadians(360.0f));
	ImGui::SliderFloat("YRotate", &PosSizeRotate._32,  0.0f,DirectX::XMConvertToRadians(360.0f));
	ImGui::SliderFloat("ZRotate", &PosSizeRotate._33,  0.0f,DirectX::XMConvertToRadians(360.0f));
	if(ImGui::Button("Reset"))
	{
		PosSizeRotate = {};
	}

	// 描画後の処理
	ImGui::End();

	return PosSizeRotate;
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

void ApperCollisionSquare(DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size, std::string name,bool* bApper, DirectX::XMFLOAT2 guipos, DirectX::XMFLOAT4  color)
{

	// 描画前の設定
	// ウィンドウを出す位置
	ImGui::SetNextWindowPos(ImVec2(guipos.x, guipos.y));
	// ウィンドウのサイズ
	ImGui::SetNextWindowSize(ImVec2(200, 150));
	// ウィンドウの名前
	ImGui::Begin(name.c_str());

	pos.y *= -1;
	ImVec2 vec2Min(pos.x - size.x / 2.0f + SCREEN_WIDTH / 2.0f, pos.y - size.y / 2.0f + SCREEN_HEIGHT / 2.0f);  // 矩形の左上座標
	ImVec2 vec2Max(pos.x + size.x / 2.0f + SCREEN_WIDTH / 2.0f, pos.y + size.y / 2.0f + SCREEN_HEIGHT / 2.0f);  // 矩形の右下座標
	ImDrawList* pDrawList = ImGui::GetForegroundDrawList();
	
	// チェックボックスで当たり判定の表示を切り替える
	ImGui::Checkbox("Apper", bApper);

	if (*bApper)
	{
		pDrawList->AddRectFilled(vec2Min, vec2Max, IM_COL32(color.x, color.y, color.z, color.w));
	}

	// 描画後の処理
	ImGui::End();
}

void ApperCollisionCircle(DirectX::XMFLOAT2 pos, float radius, std::string name, bool* bApper, DirectX::XMFLOAT2 guipos, DirectX::XMFLOAT4 color)
{

	// 描画前の設定
	// ウィンドウを出す位置
	ImGui::SetNextWindowPos(ImVec2(guipos.x, guipos.y));
	// ウィンドウのサイズ
	ImGui::SetNextWindowSize(ImVec2(200, 150));
	// ウィンドウの名前
	ImGui::Begin(name.c_str());

	pos.y *= -1;

	pos.x += SCREEN_WIDTH / 2.0f;
	pos.y += SCREEN_HEIGHT / 2.0f;

	ImDrawList* pDrawList = ImGui::GetForegroundDrawList();

	// チェックボックスで当たり判定の表示を切り替える
	ImGui::Checkbox("Apper", bApper);

	if (*bApper)
	{
		pDrawList->AddCircleFilled(ImVec2(pos.x, pos.y), radius, IM_COL32(color.x, color.y, color.z, color.w));
	}

	// 描画後の処理
	ImGui::End();
}

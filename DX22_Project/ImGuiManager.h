/*+===================================================================
	File: ImGuiManager.cpp
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
#pragma once
#include "imgui.h"
#include <DirectXMath.h>
#include <string>

// ==============================
//    マクロ定義
// ==============================
// このヘッダーの関数を使う際は、処理を#ifdef _IMGUIで囲うこと
// コメントアウトを付けないとImGuiが起動する
// #define _IMGUI

/// <summary> SpriteParamDebug:スプライトの座標、サイズをスライダーで調整する </summary>
/// <param name="pos:"> スプライトの座標 </param>
/// <param name="size:"> スプライトのサイズ </param>
/// <param name="name:"> ウィンドウの名前 </param>
/// <param name="guipos:"> ウィンドウの座標 </param>
void SpriteParamDebug(DirectX::XMFLOAT2* pos, DirectX::XMFLOAT2* size, std::string name, DirectX::XMFLOAT2 guipos = { 20.0f,20.0f });

/// <summary> ValueDebug:int型の数値をボタンで調整する </summary>
/// <param name="value:"> 調整したい値(int) </param>
/// <param name="name:"> ウィンドウの名前 </param>
/// <param name="guipos:"> ウィンドウの座標 </param>
void ValueDebug(int* value,std::string name, DirectX::XMFLOAT2 guipos = { 20.0f,20.0f });

/// <summary> BoolDebug:bool型の真偽値をボタンで調整する </summary>
/// <param name="value:"> 調整したい値(bool) </param>
/// <param name="name:"> ウィンドウの名前 </param>
/// <param name="guipos:"> ウィンドウの座標 </param>
void BoolDebug(bool* value, std::string name, DirectX::XMFLOAT2 guipos = { 20.0f,20.0f });
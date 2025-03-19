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
//#define _COLLISION_DEBUG

/// <summary> SpriteParamDebug:スプライトの座標、サイズをスライダーで調整する </summary>
/// <param name="pos:"> スプライトの座標 </param>
/// <param name="size:"> スプライトのサイズ </param>
/// <param name="name:"> ウィンドウの名前 </param>
/// <param name="guipos:"> ウィンドウの座標 </param>
void SpriteParamDebug(DirectX::XMFLOAT2* pos, DirectX::XMFLOAT2* size, std::string name, DirectX::XMFLOAT2 guipos = { 20.0f,20.0f });

/// <summary> SpriteParamDebug:スプライトの座標、サイズをスライダーで調整し、戻り値として受け取る </summary>
/// <param name="name:"> ウィンドウの名前 </param>
/// <param name="guipos:"> ウィンドウの座標 </param>
/// <returns> x,y = 座標(x,y), z,w = サイズ(x,y) </returns>
DirectX::XMFLOAT4 GetPosSizeDebug(std::string name, DirectX::XMFLOAT2 guipos = { 20.0f,20.0f });

/// <summary> GetPosSizeRotateDebug:モデルの座標、サイズ、回転をスライダーで調整し、戻り値として受け取る </summary>
/// <param name="name:"> ウィンドウの名前 </param>
/// <param name="guipos:"> ウィンドウの座標 </param>
/// <returns> _11,_12,_13 = 座標(x,y,z), _21,_22,_23 = サイズ(x,y,z), _31,_32,_33 = 回転(x,y,z) </returns>
DirectX::XMFLOAT3X3 GetPosSizeRotateDebug(std::string name, DirectX::XMFLOAT2 guipos = { 20.0f,20.0f });

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

/// <summary> ApperCollisionSprite:ボタンで短形当たり判定を表示する </summary>
/// <param name="name:"> ウィンドウの名前 </param>
/// <param name="guipos:"> ウィンドウの座標 </param>
/// <returns> x,y = 座標(x,y), z,w = サイズ(x,y) </returns>
void ApperCollisionSquare(DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size, std::string name, bool* bApper, DirectX::XMFLOAT2 guipos = { 20.0f,20.0f }, DirectX::XMFLOAT4  color = {255.0f,0.0f,0.0f,50.0f});

/// <summary> ApperCollisionSprite:ボタンで短形当たり判定を表示する </summary>
/// <param name="name:"> ウィンドウの名前 </param>
/// <param name="guipos:"> ウィンドウの座標 </param>
/// <returns> x,y = 座標(x,y), z,w = サイズ(x,y) </returns>
void ApperCollisionCircle(DirectX::XMFLOAT2 pos, float radius, std::string name, bool* bApper, DirectX::XMFLOAT2 guipos = { 20.0f,20.0f }, DirectX::XMFLOAT4  color = {255.0f,0.0f,0.0f,50.0f});
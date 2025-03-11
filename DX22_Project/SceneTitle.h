#pragma once

#include "Scene.h"
#include "Texture.h"
#include "Camera.h"
#include "DirectWriteSample.h"

class CSceneTitle : public CScene 
{
public:
	CSceneTitle();
	~CSceneTitle();

	// 更新処理 
	void Update() final;

	// 描画処理 
	void Draw() final;

private:
	void DrawStar();
	void DrawBlack();

	float m_Angle;
	Texture* m_pLogo; // タイトル画面に表示する画像
	Texture* m_pTran[2]; // タイトル画面に表示する画像
	DirectX::XMFLOAT2 m_StarPos;
	DirectX::XMFLOAT2 m_StarSize;
	DirectX::XMFLOAT2 m_BlackPos[4];
	DirectX::XMFLOAT2 m_BlackSize[4];
	DirectWriteSample* font;
};
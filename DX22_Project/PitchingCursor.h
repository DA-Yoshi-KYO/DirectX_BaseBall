#pragma once
#include "Defines.h"
#include "Collision.h"
#include "RendererComponent.h"
#include "GameObject.h"

constexpr DirectX::XMFLOAT3 ce_fPitchingCursorPos = { 0.0f,-150.0f,0.0f };	// 投球カーソルの初期位置
constexpr int ce_nMaxBenderQuality = 7;	// 細大変化量

class CPitchingCursor : public CGameObject
{
public:
	CPitchingCursor();
	~CPitchingCursor();
	void Update() override;
	void Draw() override;
	void SetMove(bool isMove) { m_bMove = isMove; };

private:
	RendererParam m_tBallParam;
	RendererParam m_tPredParam;
	bool m_bMove;	//  カーソルを動かせるか
	Collision::Info2D m_Collision;	// カーソルの当たり判定

};

#pragma once
#include "GameObject.h"

constexpr DirectX::XMFLOAT3 ce_fBattingCursorPos = { 0.0f,-150.0f,0.0f };	// 打撃カーソルの初期位置

class CBattingCursor : CGameObject
{
public:
	CBattingCursor();
	~CBattingCursor();
	void Init() override;
	void Update() override;
public:
	static Collision::Info2D GetCollision(bool isCursorOnry, Collision::Type2D type = Collision::Type2D::eCircle);
private:
	bool m_bMove;	//  カーソルを動かせるか
	static Collision::Info2D m_Collision;	// カーソルの当たり判定
};

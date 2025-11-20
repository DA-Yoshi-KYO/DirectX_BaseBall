#pragma once
#include "GameObject.h"

constexpr DirectX::XMFLOAT3 ce_fBattingCursorPos = { 0.0f,-150.0f,0.0f };	// 打撃カーソルの初期位置

class CBattingCursor : public CGameObject
{
public:
	CBattingCursor();
	~CBattingCursor();
	void Init() override;
	void Update() override;
private:
	bool m_bMove;	//  カーソルを動かせるか
};

// ==============================
//    インクルード部
// ==============================
#pragma once
#include "Defines.h"
#include "BattingCursor.h"
#include "Ball.h"

class CBatting
{
public:
	CBatting();
	~CBatting();
	void Update(int AttackPlayer);
	void Draw();

public:
	DirectX::XMFLOAT3 GetDirection();
	bool GetBatting();

private:
	// メンバ変数
	DirectX::XMFLOAT3 m_fMoveDirection;	// 打球の進行方向
	bool m_bBatting;	// バットに当たったか
	float m_fPower;		// 打者のパワー

	// 静的メンバ変数
	bool m_bSwing;	// スイングしたか
	int m_nTakingBatterNo[2];	// 打順
	void CheckHit();
};

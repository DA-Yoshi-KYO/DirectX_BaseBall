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
	// ------------Getter------------
	/// <summary> GetDirection:打球の進行方向を取得する </summary>
	/// <returns> 打球の進行方向 </returns>
	DirectX::XMFLOAT3 GetDirection();
	/// <summary> GetBatting:バットにボールが当たったか </summary>
	/// <returns> true:当たった,false:当っていない </returns>
	bool GetBatting();
	/// <summary> GetSwing:スイングを行ったか </summary>
	/// <returns> true:スイングした,false:スイングしていない </returns>
	bool GetSwing();

private:
	// メンバ変数
	DirectX::XMFLOAT3 m_fMoveDirection;	// 打球の進行方向
	bool m_bBatting;	// バットに当たったか
	float m_fPower;		// 打者のパワー

	// 静的メンバ変数
	bool m_bSwing;	// スイングしたか
};

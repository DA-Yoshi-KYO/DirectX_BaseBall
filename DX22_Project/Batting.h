#pragma once

#include "Defines.h"
#include "BattingCursor.h"
#include "Ball.h"

class CBatting
{
public:
	CBatting();
	~CBatting();
	void Update();
	void Draw();

	void SetCursor(CBattingCursor* cursor);
	void SetBall(CBall* ball);
	DirectX::XMFLOAT3 GetDirection();
	bool GetBatting();
	void SetBatting(bool isBatting);
private:
	std::unique_ptr<CBattingCursor> m_pBattingCursor;
	std::unique_ptr<CBall> m_pBall;
	DirectX::XMFLOAT3 m_fMoveDirection;
	bool m_bSwing;
	bool m_bBatting;
	float m_fPower;
};

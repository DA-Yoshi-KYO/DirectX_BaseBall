#pragma once

#include "Defines.h"
#include "Cursor.h"
#include "Ball.h"

class CBatting
{
public:
	CBatting();
	~CBatting();
	void Update();
	void Draw();

	void SetCursor(CCursor* cursor);
	void SetBall(CBall* ball);
private:
	std::unique_ptr<CCursor> m_pCursor;
	std::unique_ptr<CBall> m_pBall;

	bool m_bSwing;
};

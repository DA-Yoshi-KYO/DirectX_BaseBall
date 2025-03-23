#pragma once

#include "Defines.h"
#include "BattingCursor.h"
#include "Batting.h"

class CAttack
{
public:
	CAttack();
	~CAttack();
	void Update();
	void Draw();
private:
	std::unique_ptr<CBattingCursor> m_pBattingCursor;
	std::unique_ptr<CBatting> m_pBatting;
};
#pragma once

#include "Defines.h"
#include "StrikeZone.h"
#include "Cursor.h"

class CAttack
{
public:
	CAttack();
	~CAttack();
	void Update();
	void Draw();
private:
	std::unique_ptr<CStrikeZone> m_pStrikeZone;
	std::unique_ptr<CCursor> m_pCursor;

};
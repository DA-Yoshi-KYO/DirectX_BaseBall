#pragma once

#include "Defines.h"
#include "Cursor.h"
#include "StrikeZone.h"
#include "Pitching.h"

class CDefence
{
public:
	CDefence();
	~CDefence();
	void Update();
	void Draw();
private:
	std::unique_ptr<CStrikeZone> m_pStrikeZone;
	std::unique_ptr<CCursor> m_pCursor;
	std::unique_ptr<CPitching> m_pPitching;
};

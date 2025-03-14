#pragma once

#include "Defines.h"
#include "StrikeZone.h"
#include "Cursor.h"

class CPitching
{
public:
	CPitching();
	~CPitching();
	void Update();
	void Draw();
	void SetStrikeZone(CStrikeZone* zone);
	void SetCursor(CCursor* cursor);
private:
	std::unique_ptr<CStrikeZone> m_pStrikeZone;
	std::unique_ptr<CCursor> m_pCursor;
	
	int m_nPitchingPhase;
	float m_fSpeed;
	float m_fChatchTime;

	enum class PitchingPhase
	{
		Set,
		Pitch,
		Release,
	};
};
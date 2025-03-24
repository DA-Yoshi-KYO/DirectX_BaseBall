#pragma once

#include "Defines.h"
#include "PitchingCursor.h"
#include "Pitching.h"
#include "Fielding.h"

class CDefence
{
public:
	CDefence();
	~CDefence();
	void Update();
	void Draw();
private:
	std::unique_ptr<CPitchingCursor> m_pPitchingCursor;
	std::unique_ptr<CPitching> m_pPitching;
	std::unique_ptr<CFielding> m_pFielding;
};

#pragma once

#include "Defines.h"
#include "PitchingCursor.h"
#include "Pitching.h"

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
};

#pragma once

#include "StrikeZone.h"
#include "Defines.h"

class CAttack
{
public:
	CAttack();
	~CAttack();
	void Update();
	void Draw();
private:
	std::unique_ptr<CStrikeZone> m_pStrikeZone;
};
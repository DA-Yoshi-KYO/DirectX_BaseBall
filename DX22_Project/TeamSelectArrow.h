#pragma once

#include "GameObject.h"

class CTeamSelectArrow : public CGameObject
{
public:
	CTeamSelectArrow();
	~CTeamSelectArrow();
	void Init() override;
	void SetIsUp(bool isUp);

};


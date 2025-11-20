#pragma once

#include "GameObject.h"
#include "PlayerData.h"

class CPositionIcon : public CGameObject
{
public:
	CPositionIcon();
	~CPositionIcon();
	void Init() override;
	void Init(Positions position);

};


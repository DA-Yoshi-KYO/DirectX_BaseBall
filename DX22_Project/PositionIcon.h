#pragma once

#include "GameObject.h"
#include "PlayerDataBase.h"

class CPositionIcon : public CGameObject
{
public:
	CPositionIcon();
	~CPositionIcon();
	void Init() override;
	void Init(Positions position);

};


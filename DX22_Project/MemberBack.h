#pragma once

#include "GameObject.h"
#include "PlayerDataBase.h"

class CMemberBack : public CGameObject
{
public:
	CMemberBack();
	~CMemberBack();
	void Init() override {}
	void Init(Positions position);

};


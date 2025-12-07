#pragma once

#include "GameDirecterBase.h"

class CFieldDirecter : public CGameDirecterBase
{
public:
	CFieldDirecter();
	~CFieldDirecter();
	void Init() override;
	void EndInplay() override;

};

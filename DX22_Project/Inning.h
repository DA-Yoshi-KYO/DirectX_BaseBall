#pragma once

#include "GameObject.h"

class CInning : public CGameObject
{
public:
	CInning();
	~CInning();
	void Init() override;
	void Draw() override;
	void InningProgress();

private:
	int m_nInningNum;
	bool m_bIsStart;

};


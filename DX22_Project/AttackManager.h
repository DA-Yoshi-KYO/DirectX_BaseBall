#pragma once

#include <memory>
#include "Batting.h"

class CAttackManager
{
public:
	CAttackManager();
	~CAttackManager();
	void Init();
	void Update();
	void SetPlayerNo(int no) { m_nPlayerNo = no; }
	int GetPlayerNo() { return m_nPlayerNo; }
	CBatting* GetBatting() { return m_pBatting.get(); }

private:
	int m_nPlayerNo;
	std::unique_ptr<CBatting> m_pBatting;

};

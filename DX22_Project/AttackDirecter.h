#pragma once

#include <memory>
#include "GameDirecterBase.h"
#include "Batting.h"
#include "Running.h"

class CAttackDirecter : public CGameDirecterBase
{
public:
	CAttackDirecter();
	~CAttackDirecter();
	void Init() override;
	void Update();
	void EndInplay() override;

	void SetPlayerNo(int no) { m_nPlayerNo = no; }
	int GetPlayerNo() { return m_nPlayerNo; }
	CBatting* GetBatting() { return m_pBatting.get(); }
	CRunning* GetRunning() { return m_pRunning.get(); }

private:
	int m_nPlayerNo;
	std::unique_ptr<CBatting> m_pBatting;
	std::unique_ptr<CRunning> m_pRunning;

};

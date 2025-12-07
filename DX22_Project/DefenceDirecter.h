#pragma once

#include "GameDirecterBase.h"
#include "Pitching.h"
#include "Fielding.h"
#include <memory>

class CDefenceDirecter : public CGameDirecterBase
{
public:
	CDefenceDirecter();
	~CDefenceDirecter();
	void Init() override;
	void Update();
	void EndInplay() override;
	
	void SetPlayerNo(int no) { m_nPlayerNo = no; }
	int GetPlayerNo() { return m_nPlayerNo; }
	CPitching* GetPitching() { return m_pPitching.get(); }

private:
	int m_nPlayerNo;
	std::unique_ptr<CPitching> m_pPitching;
	std::unique_ptr<CFielding> m_pFielding;

};

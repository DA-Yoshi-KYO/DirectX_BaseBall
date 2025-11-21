#pragma once

#include "Pitching.h"
#include <memory>

class CDefenceManager
{
public:
	CDefenceManager();
	~CDefenceManager();
	void Init();
	void SetPlayerNo(int no) { m_nPlayerNo = no; }
	int GetPlayerNo() { return m_nPlayerNo; }
	CPitching* GetPitching() { return m_pPitching.get(); }

private:
	int m_nPlayerNo;
	std::unique_ptr<CPitching> m_pPitching;

};
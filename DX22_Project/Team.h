#pragma once

#include <list>

enum TeamKind
{
	Bears,
	Rabbits,
	Tigers,
	Elephants,
	Monkeys,

	Max
};

class CTeam
{
public:
	CTeam();
	~CTeam();
	void Load(TeamKind team);
	std::list<class CPlayerDataBase*> GetAllMember() { return m_pMemberData; }
	std::list<class CPitcherData*> GetPitcherMember() { return m_pPitcherData; }
	std::list<class CFielderData*> GetFielderMember() { return m_pFielderData; }
	std::list<class CChatcherData*> GetChatcherMember() { return m_pChatcherData; }
	std::list<class CInFielderData*> GetInFielderMember() { return m_pInFielderData; }
	std::list<class COutFielderData*> GetOutFielderMember() { return m_pOutFielderData; }

private:
	std::list<CPlayerDataBase*> m_pMemberData;
	std::list<CPitcherData*> m_pPitcherData;
	std::list<CFielderData*> m_pFielderData;
	std::list<CChatcherData*> m_pChatcherData;
	std::list<CInFielderData*> m_pInFielderData;
	std::list<COutFielderData*> m_pOutFielderData;

public:
	void SetStarterPitcher(CPitcherData* starter) { m_pStarterPitcher = starter; }
	CPitcherData* GetStarterPitcher() { return m_pStarterPitcher; }
	void SetStartingLineup(std::list<CFielderData*> lineup) { m_pStartingLineup = lineup; }
	std::list<CFielderData*> GetStartingLineup() { return m_pStartingLineup; }

	CFielderData* GetTakingBatter(int TakingNo);
	CPitcherData* GetTakingPitcher() { return m_pTakingPitcher; }
	void SetTakingPitcher(CPitcherData* pitcher) { m_pTakingPitcher = pitcher; }

private:
	CPitcherData* m_pStarterPitcher;
	std::list<CFielderData*> m_pStartingLineup;
	CPitcherData* m_pTakingPitcher;

};
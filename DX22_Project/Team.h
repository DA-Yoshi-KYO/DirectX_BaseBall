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
	std::list<class CPitcher*> GetPitcherMember() { return m_pPitcherData; }
	std::list<class CFielder*> GetFielderMember() { return m_pFielderData; }
	std::list<class CChatcher*> GetChatcherMember() { return m_pChatcherData; }
	std::list<class CInFielder*> GetInFielderMember() { return m_pInFielderData; }
	std::list<class COutFielder*> GetOutFielderMember() { return m_pOutFielderData; }

private:
	std::list<class CPlayerDataBase*> m_pMemberData;
	std::list<class CPitcher*> m_pPitcherData;
	std::list<class CFielder*> m_pFielderData;
	std::list<class CChatcher*> m_pChatcherData;
	std::list<class CInFielder*> m_pInFielderData;
	std::list<class COutFielder*> m_pOutFielderData;

public:
	void SetStarterPitcher(CPitcher* starter) { m_pStarterPitcher = starter; }
	CPitcher* GetStarterPitcher() { return m_pStarterPitcher; }
	void SetStartingLineup(std::list<class CFielder*> lineup) { m_pStartingLineup = lineup; }
	std::list<class CFielder*> GetStartingLineup() { return m_pStartingLineup; }

	CFielder* GetTakingBatter(int TakingNo);
	CPitcher* GetTakingPitcher() { return m_pTakingPitcher; }
	void SetTakingPitcher(CPitcher* pitcher) { m_pTakingPitcher = pitcher; }

private:
	CPitcher* m_pStarterPitcher;
	std::list<class CFielder*> m_pStartingLineup;
	CPitcher* m_pTakingPitcher;

};
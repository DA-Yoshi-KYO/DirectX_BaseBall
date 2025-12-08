#pragma once

#include <list>
#include "PlayerDataBase.h"
#include "PitcherData.h"
#include "FielderData.h"
#include "ChatcherData.h"
#include "InFielderData.h"
#include "OutFielderData.h"

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
	std::list<CPlayerDataBase*> GetAllMember() { return m_pMemberData; }
	std::list<CPitcherData*> GetPitcherMember() { return m_pPitcherData; }
	std::list<CFielderData*> GetFielderMember() { return m_pFielderData; }
	std::list<CChatcherData*> GetChatcherMember() { return m_pChatcherData; }
	std::list<CInFielderData*> GetInFielderMember() { return m_pInFielderData; }
	std::list<COutFielderData*> GetOutFielderMember() { return m_pOutFielderData; }

private:
	std::list<CPlayerDataBase*> m_pMemberData;
	std::list<CPitcherData*> m_pPitcherData;
	std::list<CFielderData*> m_pFielderData;
	std::list<CChatcherData*> m_pChatcherData;
	std::list<CInFielderData*> m_pInFielderData;
	std::list<COutFielderData*> m_pOutFielderData;

public:
	void SetStarterPitcher(CPitcherData* starter) { m_pStarterPitcher = starter; }
	void SetStartingLineup(std::list<CFielderData*> lineup) { m_pStartingLineup = lineup; }
	void SetTakingPitcher(CPitcherData* pitcher) { m_pTakingPitcher = pitcher; }

	CPitcherData* GetStarterPitcher() { return m_pStarterPitcher; }
	CFielderData* GetTakingBatter() { return m_pTakingBatter; }
	CPitcherData* GetTakingPitcher() { return m_pTakingPitcher; }
	CFielderData* GetPositionFielder(Positions position);
	std::list<CFielderData*> GetStartingLineup() { return m_pStartingLineup; }

	void NextBatter();

private:
	CPitcherData* m_pStarterPitcher;
	std::list<CFielderData*> m_pStartingLineup;
	CPitcherData* m_pTakingPitcher;
	CFielderData* m_pTakingBatter;
	int m_nTakingBatterNo;

};
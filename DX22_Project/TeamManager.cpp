#include "TeamManager.h"
#include <algorithm>
#include "DirectX.h"
#include "Pitcher.h"
#include "Fielder.h"
#include "Chatcher.h"
#include "InFielder.h"
#include "OutFielder.h"
#include <vector>

std::array<CTeam*, 2>  CTeamDirector::m_pTeam = {};

CTeamDirector::CTeamDirector(int PlayerNo)
{
    m_nPlayerNo = PlayerNo - 1;
    m_nPlayerNo = std::clamp(m_nPlayerNo, 0, 1);

    for (auto i = 0; i < m_pTeam.size(); i++)
    {
        if (m_nPlayerNo == i && m_pTeam[i]) m_pTeam[i] = new CTeam();
    }
}

CTeamDirector::~CTeamDirector()
{

}

void CTeamDirector::TeamInit()
{
    InitStarter();
    InitStartingLineup();
}

void CTeamDirector::TeamUninit()
{
    for (auto itr : m_pTeam)
    {
        SAFE_DELETE(itr);
    }
}

CTeam* CTeamDirector::GetTeam()
{
    return m_pTeam[m_nPlayerNo];
}

void CTeamDirector::InitStarter()
{
    std::list<CPitcher*> pPitcherList = m_pTeam[m_nPlayerNo]->GetPitcherMember();

    std::vector<CPitcher*> pStarterList;
    pStarterList.clear();

    for (auto itr : pPitcherList)
    {
        if (itr->GetPitcherData().m_eKind == PitcherKind::Starter) pStarterList.push_back(itr);
    }

    int nRand = rand() % pPitcherList.size();
    m_pTeam[m_nPlayerNo]->SetStarterPitcher(pStarterList[m_nPlayerNo]);
}

void CTeamDirector::InitStartingLineup()
{
    std::list<CFielder*> pFielder = m_pTeam[m_nPlayerNo]->GetFielderMember();
    std::list<CFielder*> pLineupList;
    pLineupList.clear();
    for (auto itr : pFielder)
    {
        if (itr->GetPlayerData().m_nLineupNo != 0) pLineupList.push_back(itr);
    }
    pLineupList.sort([](CFielder* A, CFielder* B)
        {
            return A->GetPlayerData().m_nLineupNo < B->GetPlayerData().m_nLineupNo;
        });

    m_pTeam[m_nPlayerNo]->SetStartingLineup(pLineupList);
}

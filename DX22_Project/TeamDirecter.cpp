#include "TeamDirecter.h"
#include <algorithm>
#include "DirectX.h"
#include "PitcherData.h"
#include "FielderData.h"
#include "ChatcherData.h"
#include "InFielderData.h"
#include "OutFielderData.h"
#include <vector>

std::array<CTeam*, 2>  CTeamDirector::m_pTeam = {};

CTeamDirector::CTeamDirector(int PlayerNo)
    : m_nPlayerIndex(-1)
{
    m_nPlayerIndex = PlayerNo - 1;
    m_nPlayerIndex = std::clamp(m_nPlayerIndex, 0, 1);

    for (auto i = 0; i < m_pTeam.size(); i++)
    {
        if (m_nPlayerIndex == i && !m_pTeam[i]) m_pTeam[i] = new CTeam();
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
    return m_pTeam[m_nPlayerIndex];
}

void CTeamDirector::EndInplay(bool isAttack)
{
    if (isAttack)
    {
        m_pTeam[m_nPlayerIndex]->NextBatter();
    }
    else
    {

    }
}

void CTeamDirector::InitStarter()
{
    std::list<CPitcherData*> pPitcherList = m_pTeam[m_nPlayerIndex]->GetPitcherMember();

    std::vector<CPitcherData*> pStarterList;
    pStarterList.clear();

    for (auto itr : pPitcherList)
    {
        if (itr->GetPitcherData().m_eKind == PitcherKind::Starter) pStarterList.push_back(itr);
    }

    int nRand = rand() % pPitcherList.size();
    m_pTeam[m_nPlayerIndex]->SetStarterPitcher(pStarterList[m_nPlayerIndex]);
    m_pTeam[m_nPlayerIndex]->SetTakingPitcher(pStarterList[m_nPlayerIndex]);
}

void CTeamDirector::InitStartingLineup()
{
    std::list<CFielderData*> pFielder = m_pTeam[m_nPlayerIndex]->GetFielderMember();
    std::list<CFielderData*> pLineupList;
    pLineupList.clear();
    for (auto itr : pFielder)
    {
        if (itr->GetPlayerData().m_nLineupNo != 0) pLineupList.push_back(itr);
    }
    pLineupList.sort([](CFielderData* A, CFielderData* B)
        {
            return A->GetPlayerData().m_nLineupNo < B->GetPlayerData().m_nLineupNo;
        });

    m_pTeam[m_nPlayerIndex]->SetStartingLineup(pLineupList);
}

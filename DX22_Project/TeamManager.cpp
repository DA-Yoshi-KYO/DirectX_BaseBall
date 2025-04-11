#include "TeamManager.h"
#include "TeamBears.h"

CTeamManager::CTeamManager()
{
}

CTeamManager::~CTeamManager()
{
}

std::vector<CTeamManager::PitcherState> CTeamManager::GetPitcherState()
{
    return m_tVecPitcherMember;
}

std::vector<CTeamManager::BatterState> CTeamManager::GetBatterState()
{
    return m_tVecBatterMember;
}

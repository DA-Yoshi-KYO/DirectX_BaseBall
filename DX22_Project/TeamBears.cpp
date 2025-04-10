#include "TeamBears.h"

void CTeamBears::Init()
{
	m_tVecLineup.erase(m_tVecLineup.begin(), m_tVecLineup.end());
	m_tVecBatterMember.erase(m_tVecBatterMember.begin(), m_tVecBatterMember.end());
	m_tVecPitcherMember.erase(m_tVecPitcherMember.begin(), m_tVecPitcherMember.end());
}

void CTeamBears::Load()
{
}

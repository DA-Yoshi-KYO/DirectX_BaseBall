#include "SceneTeamselect.h"

int CSceneTeamSelect::m_nTeam[2] = { 0, 0 };

CSceneTeamSelect::CSceneTeamSelect()
{
}

CSceneTeamSelect::~CSceneTeamSelect()
{
}

void CSceneTeamSelect::Update()
{
}

void CSceneTeamSelect::Draw()
{
}

CTeamManager::Teams CSceneTeamSelect::GetTeam(int TeamNo)
{
	return (CTeamManager::Teams)m_nTeam[TeamNo];
}

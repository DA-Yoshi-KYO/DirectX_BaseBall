#pragma once
#include "Scene.h"
#include "TeamManager.h"

class CSceneMemberselect : public CScene
{
public:
	CSceneMemberselect(CTeamManager::Teams player1 , CTeamManager::Teams player2);
	~CSceneMemberselect();

private:
	std::unique_ptr<CTeamManager> m_pTeam[2];
};

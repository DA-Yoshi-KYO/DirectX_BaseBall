#pragma once

#include "TeamManager.h"
#include "SelectTeam.h"
#include "TeamSelectBackGround.h"
#include <array>

class CTeamSelectDirector
{
public:
	CTeamSelectDirector();
	~CTeamSelectDirector();
	void Init();
	void Update();
	static CTeamManager::Teams GetTeam(int TeamNo);

private:
	std::array<CSelectTeam*, CTeamManager::Teams::Max> m_pTeams1;
	std::array<CSelectTeam*, CTeamManager::Teams::Max> m_pTeams2;
	CTeamSelectBackGround* m_pBackGround;
	bool IsSelectedPlayer1;
	bool IsSelectedPlayer2;
	static CTeamManager::Teams m_eTeam1;
	static CTeamManager::Teams m_eTeam2;

};


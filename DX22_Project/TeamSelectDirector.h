#pragma once

#include "Team.h"
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
	static TeamKind GetTeam(int TeamNo);

private:
	std::array<CSelectTeam*, TeamKind::Max> m_pTeams1;
	std::array<CSelectTeam*, TeamKind::Max> m_pTeams2;
	CTeamSelectBackGround* m_pBackGround;
	bool IsSelectedPlayer1;
	bool IsSelectedPlayer2;
	static TeamKind m_eTeam1;
	static TeamKind m_eTeam2;

};


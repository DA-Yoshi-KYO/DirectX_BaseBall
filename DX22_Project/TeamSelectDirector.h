#pragma once

#include "Team.h"
#include "SelectTeam.h"
#include "TeamSelectBackGround.h"
#include "TeamSelectArrow.h"
#include "TeamSelectSE.h"
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
	std::array<CTeamSelectArrow*, 2> m_pArrow1;
	std::array<CTeamSelectArrow*, 2> m_pArrow2;
	CTeamSelectBackGround* m_pBackGround;
	CTeamSelectSE* m_pSEs;
	bool m_bSelectedPlayer1;
	bool m_bSelectedPlayer2;
	static TeamKind m_eTeam1;
	static TeamKind m_eTeam2;
	bool m_bEnd;

};

#pragma once

#include "MemberIcon.h"
#include "TeamManager.h"
#include <memory>
#include <vector>
#include <array>

class CMemberSelectDirector
{
public:
	CMemberSelectDirector();
	~CMemberSelectDirector();
	void Init(TeamKind kind1, TeamKind kind2);

private:
	std::vector<std::unique_ptr<CMemberIcon>> m_pIconsTeam1;
	std::vector<std::unique_ptr<CMemberIcon>> m_pIconsTeam2;
	std::array<std::unique_ptr<CTeamDirector>,2> m_pTeams;

};

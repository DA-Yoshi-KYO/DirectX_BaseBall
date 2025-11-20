#pragma once

#include "MemberIcon.h"
#include "TeamManager.h"
#include <memory>
#include <vector>
#include <array>

enum class MemberKind
{
	StarterPitcher,
	ReliefPitcher,
	RestStarterPitcher,
	StarterFielder,
	BenchFielder,

	Max
};

class CMemberSelectDirector
{
public:
	CMemberSelectDirector();
	~CMemberSelectDirector();
	void Init(TeamKind kind1, TeamKind kind2);
	void Update();

private:
	std::array<std::vector<std::unique_ptr<CMemberIcon>>, (int)MemberKind::Max> m_pIconsTeam1;
	std::array<std::vector<std::unique_ptr<CMemberIcon>>, (int)MemberKind::Max> m_pIconsTeam2;
	std::array<std::unique_ptr<CTeamDirector>,2> m_pTeams;

};

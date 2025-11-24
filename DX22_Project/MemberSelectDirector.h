#pragma once

#include "MemberIcon.h"
#include "TeamManager.h"
#include <memory>
#include <vector>
#include <array>
#include "StartingLineupField.h"
#include "SelectPitcherField.h"
#include "SelectFielderField.h"
#include "PositionIcon.h"

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
	std::array<std::unique_ptr<CTeamDirector>,2> m_pTeams;
	std::array<std::unique_ptr<CStartingLineupField>, 2> m_pStartingLineup;
	std::array<std::unique_ptr<CSelectPitcherField>, 2> m_pBenchPitcher;
	std::array<std::unique_ptr<CSelectFielderField>, 2> m_pBenchFielder;
	std::array<std::array<CPositionIcon*, 9>, 2> m_pPositionLineup;
	std::array<CPositionIcon*, 2> m_pPitcherIcon;
};

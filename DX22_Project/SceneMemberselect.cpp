#include "SceneMemberselect.h"
#include "TeamBears.h"
#include "TeamRabbits.h"
#include "TeamTigers.h"
#include "TeamElephants.h"
#include "TeamMonkeys.h"

std::unique_ptr<CTeamManager> CSceneMemberselect::m_pTeam[2] = { nullptr,nullptr };

CSceneMemberselect::CSceneMemberselect(CTeamManager::Teams player1, CTeamManager::Teams player2)
{
	InitMember(player1, 0);
	InitMember(player2, 1);

	for (int i = 0; i < 2; i++)
	{
		m_pTeam[i]->Load();
	}
}

CSceneMemberselect::~CSceneMemberselect()
{
}

void CSceneMemberselect::Update()
{
}

void CSceneMemberselect::Draw()
{
}

std::unique_ptr<CTeamManager>& CSceneMemberselect::GetInstance(int teamNo)
{
	static std::unique_ptr<CTeamManager> pTeamNullptr = nullptr;

	if (!m_pTeam[teamNo])
	{
		return pTeamNullptr;
	}

	return m_pTeam[teamNo];
}

void CSceneMemberselect::InitMember(CTeamManager::Teams team, int teamNo)
{
	switch (team)
	{
	case CTeamManager::Bears:
		m_pTeam[teamNo] = std::make_unique<CTeamBears>();
		break;
	case CTeamManager::Rabbits:
		m_pTeam[teamNo] = std::make_unique<CTeamRabbits>();
		break;
	case CTeamManager::Tigers:
		m_pTeam[teamNo] = std::make_unique<CTeamTigers>();
		break;
	case CTeamManager::Elephants:
		m_pTeam[teamNo] = std::make_unique<CTeamElephants>();
		break;
	case CTeamManager::Monkeys:
		m_pTeam[teamNo] = std::make_unique<CTeamMonkeys>();
		break;
	case CTeamManager::Max:
		break;
	default:
		break;
	}
}

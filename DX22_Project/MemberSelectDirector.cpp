#include "MemberSelectDirector.h"
#include "Main.h"
#include "MemberSelectBackGround.h"
#include "PitcherData.h"
#include "FielderData.h"

CMemberSelectDirector::CMemberSelectDirector()
	: m_pIconsTeam1{}, m_pIconsTeam2{}
{

}

CMemberSelectDirector::~CMemberSelectDirector()
{

}

void CMemberSelectDirector::Init(TeamKind kind1, TeamKind kind2)
{
	CScene* pScene = GetScene();

	pScene->AddGameObject<CMemberSelectBackGround>("BackGround", Tag::UI);

	m_pTeams[0] = std::make_unique<CTeamDirector>(1);
	CTeam* pTeam1 = m_pTeams[0]->GetTeam();
	pTeam1->Load(kind1);
	for (auto itr : pTeam1->GetAllMember())
	{
		if (dynamic_cast<CPitcherData*>(itr))
		{
			if (dynamic_cast<CPitcherData*>(itr) == pTeam1->GetStarterPitcher())
			{
				m_pIconsTeam1[(int)MemberKind::StarterPitcher].push_back(std::make_unique<CMemberIcon>());
				m_pIconsTeam1[(int)MemberKind::StarterPitcher].rbegin()->get()->Init(itr);
			}
			else
			{
				m_pIconsTeam1[(int)MemberKind::RestStarterPitcher].push_back(std::make_unique<CMemberIcon>());
				m_pIconsTeam1[(int)MemberKind::RestStarterPitcher].rbegin()->get()->Init(itr);
			}

			continue;
		}

		if (dynamic_cast<CFielderData*>(itr))
		{
			if (dynamic_cast<CFielderData*>(itr)->GetPlayerData().m_nLineupNo != 0)
			{
				m_pIconsTeam1[(int)MemberKind::StarterFielder].push_back(std::make_unique<CMemberIcon>());
				m_pIconsTeam1[(int)MemberKind::StarterFielder].rbegin()->get()->Init(itr);
			}
			else
			{
				m_pIconsTeam1[(int)MemberKind::BenchFielder].push_back(std::make_unique<CMemberIcon>());
				m_pIconsTeam1[(int)MemberKind::BenchFielder].rbegin()->get()->Init(itr);
			}

			continue;
		}
	}

	m_pTeams[1] = std::make_unique<CTeamDirector>(2);
	CTeam* pTeam2 = m_pTeams[1]->GetTeam();
	pTeam2->Load(kind2);
	for (auto itr : pTeam2->GetAllMember())
	{
		if (dynamic_cast<CPitcherData*>(itr))
		{
			if (dynamic_cast<CPitcherData*>(itr) == pTeam2->GetStarterPitcher())
			{
				m_pIconsTeam2[(int)MemberKind::StarterPitcher].push_back(std::make_unique<CMemberIcon>());
				m_pIconsTeam2[(int)MemberKind::StarterPitcher].rbegin()->get()->Init(itr);
			}
			else
			{
				m_pIconsTeam2[(int)MemberKind::RestStarterPitcher].push_back(std::make_unique<CMemberIcon>());
				m_pIconsTeam2[(int)MemberKind::RestStarterPitcher].rbegin()->get()->Init(itr);
			}

			continue;
		}

		if (dynamic_cast<CFielderData*>(itr))
		{
			if (dynamic_cast<CFielderData*>(itr)->GetPlayerData().m_nLineupNo != 0)
			{
				m_pIconsTeam2[(int)MemberKind::StarterFielder].push_back(std::make_unique<CMemberIcon>());
				m_pIconsTeam2[(int)MemberKind::StarterFielder].rbegin()->get()->Init(itr);
			}
			else
			{
				m_pIconsTeam2[(int)MemberKind::BenchFielder].push_back(std::make_unique<CMemberIcon>());
				m_pIconsTeam2[(int)MemberKind::BenchFielder].rbegin()->get()->Init(itr);
			}

			continue;
		}
	}

}

void CMemberSelectDirector::Update()
{

}

#include "MemberSelectDirector.h"
#include "Main.h"
#include "MemberSelectBackGround.h"

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
		m_pIconsTeam1.push_back(std::make_unique<CMemberIcon>());
		m_pIconsTeam1.rbegin()->get()->Init(itr);
	}

	m_pTeams[1] = std::make_unique<CTeamDirector>(2);
	CTeam* pTeam2 = m_pTeams[1]->GetTeam();
	pTeam2->Load(kind2);
	for (auto itr : pTeam2->GetAllMember())
	{
		m_pIconsTeam2.push_back(std::make_unique<CMemberIcon>());
		m_pIconsTeam2.rbegin()->get()->Init(itr);
	}
}

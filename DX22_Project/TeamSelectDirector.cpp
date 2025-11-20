#include "TeamSelectDirector.h"
#include "Main.h"
#include "Input.h"
#include "SceneMemberselect.h"

TeamKind CTeamSelectDirector::m_eTeam1 = TeamKind::Bears;
TeamKind CTeamSelectDirector::m_eTeam2 = TeamKind::Bears;

CTeamSelectDirector::CTeamSelectDirector()
	: m_pTeams1{}, m_pTeams2{}
{

}

CTeamSelectDirector::~CTeamSelectDirector()
{

}

void CTeamSelectDirector::Init()
{
	CScene* pScene = GetScene();
	m_pBackGround = pScene->AddGameObject<CTeamSelectBackGround>("BackGround", Tag::UI);

	for (int i = 0; i < m_pTeams1.size(); i++)
	{
		m_pTeams1[i] = pScene->AddGameObject<CSelectTeam>("Player1Team", Tag::UI);
		m_pTeams1[i]->Init((TeamKind)i, 1);
	}

	for (int i = 0; i < m_pTeams2.size(); i++)
	{
		m_pTeams2[i] = pScene->AddGameObject<CSelectTeam>("Player2Team", Tag::UI);
		m_pTeams2[i]->Init((TeamKind)i, 2);
	}

}

void CTeamSelectDirector::Update()
{
	if (!IsSelectedPlayer1)
	{
		if (IsKeyTrigger(1, Input::Up))
		{
			for (int i = 0; i < m_pTeams1.size(); i++)
			{
				m_pTeams1[i]->Move(true);
			}
		}
		else if (IsKeyTrigger(1, Input::Down))
		{
			for (int i = 0; i < m_pTeams1.size(); i++)
			{
				m_pTeams1[i]->Move(false);
			}
		}
		else if (IsKeyTrigger(1, Input::A))
		{
			bool isStop = true;
			int nSelectTeam = -1;
			for (int i = 0; i < m_pTeams1.size(); i++)
			{
				isStop = m_pTeams1[i]->GetIsMove();
				if (!isStop) break;
				if (m_pTeams1[i]->GetIsSelect()) nSelectTeam = i;
			}

			if (isStop)
			{
				m_eTeam1 = (TeamKind)nSelectTeam;
				IsSelectedPlayer1 = true;
			}
		}
	}

	if (!IsSelectedPlayer2)
	{
		if (IsKeyTrigger(2, Input::Up))
		{
			for (int i = 0; i < m_pTeams2.size(); i++)
			{
				m_pTeams2[i]->Move(true);
			}
		}
		else if (IsKeyTrigger(2, Input::Down))
		{
			for (int i = 0; i < m_pTeams2.size(); i++)
			{
				m_pTeams2[i]->Move(false);
			}
		}
		else if (IsKeyTrigger(2, Input::A))
		{
			bool isStop = true;
			int nSelectTeam = -1;
			for (int i = 0; i < m_pTeams2.size(); i++)
			{
				isStop = m_pTeams2[i]->GetIsMove();
				if (!isStop) break;
				if (m_pTeams2[i]->GetIsSelect()) nSelectTeam = i;
			}

			if (isStop)
			{
				m_eTeam2 = (TeamKind)nSelectTeam;
				IsSelectedPlayer2 = true;
			}
		}
	}

	if (IsSelectedPlayer1 && IsSelectedPlayer2)
	{
		FadeOut([]()
			{
				ChangeScene(new CSceneMemberselect(m_eTeam1, m_eTeam2));
				FadeIn(nullptr);
			});
	}
}

TeamKind CTeamSelectDirector::GetTeam(int TeamNo)
{
	switch (TeamNo)
	{
	case 1:return m_eTeam1; break;
	case 2:return m_eTeam2; break;
	default: return TeamKind::Bears; break;
	}
}

#include "TeamSelectDirector.h"
#include "Main.h"
#include "Input.h"
#include "SceneMemberselect.h"
#include "SceneTitle.h"

TeamKind CTeamSelectDirector::m_eTeam1 = TeamKind::Bears;
TeamKind CTeamSelectDirector::m_eTeam2 = TeamKind::Bears;

CTeamSelectDirector::CTeamSelectDirector()
	: m_pTeams1{}, m_pTeams2{}
	, m_pArrow1{}, m_pArrow2{}
	, m_pBackGround(nullptr), m_pSEs(nullptr)
	, m_bSelectedPlayer1(false), m_bSelectedPlayer2(false)
	, m_bEnd(false)
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

	m_pSEs = pScene->AddGameObject<CTeamSelectSE>("SEs", Tag::UI);

	//for (int i = 0; i < 2; i++)
	//{
	//	m_pArrow1[i] = pScene->AddGameObject<CTeamSelectArrow>("Player1Arrow", Tag::UI);
	//	m_pArrow1[i]->SetIsUp(i);
	//	m_pArrow1[i]->SetPos(DirectX::XMFLOAT3(SCREEN_WIDTH * 0.5f - 400.0f ,SCREEN_HEIGHT - 200.0f - 320.0f * i,0.0f));
	//	m_pArrow2[i] = pScene->AddGameObject<CTeamSelectArrow>("Player2Arrow", Tag::UI);
	//	m_pArrow2[i]->SetIsUp(i);
	//	m_pArrow2[i]->SetPos(DirectX::XMFLOAT3(SCREEN_WIDTH * 0.5f + 400.0f , SCREEN_HEIGHT - 200.0f - 320.0f * i,0.0f));
	//}
}

void CTeamSelectDirector::Update()
{
	if (!m_bSelectedPlayer1 && !m_pTeams1[0]->GetIsMove())
	{
		if (IsKeyTrigger(1, Input::Up))
		{
			auto SE = m_pSEs->GetSEList();
			SE["Select"]->Play();
			for (int i = 0; i < m_pTeams1.size(); i++)
			{
				m_pTeams1[i]->Move(true);
			}
		}
		else if (IsKeyTrigger(1, Input::Down))
		{
			auto SE = m_pSEs->GetSEList();
			SE["Select"]->Play();
			for (int i = 0; i < m_pTeams1.size(); i++)
			{
				m_pTeams1[i]->Move(false);
			}
		}
		else if (IsKeyTrigger(1, Input::B))
		{
			auto SE = m_pSEs->GetSEList();
			SE["Decision"]->Play();
			if (!m_bEnd)
			{
				m_bEnd = true;
				FadeOut([]()
					{
						ChangeScene(new CSceneTitle());
						FadeIn(nullptr);
					});
			}
		}
		else if (IsKeyTrigger(1, Input::A))
		{
			bool isMove = false;
			int nSelectTeam = -1;
			auto SE = m_pSEs->GetSEList();
			SE["Decision"]->Play();
			for (int i = 0; i < m_pTeams1.size(); i++)
			{
				isMove = m_pTeams1[i]->GetIsMove();
				if (isMove) break;
				if (m_pTeams1[i]->GetIsSelect()) nSelectTeam = i;
			}

			if (!isMove)
			{
				m_eTeam1 = (TeamKind)nSelectTeam;
				m_bSelectedPlayer1 = true;
			}
		}
	}

	if (!m_bSelectedPlayer2 && !m_pTeams2[0]->GetIsMove())
	{
		if (IsKeyTrigger(2, Input::Up))
		{
			auto SE = m_pSEs->GetSEList();
			SE["Select"]->Play();
			for (int i = 0; i < m_pTeams2.size(); i++)
			{
				m_pTeams2[i]->Move(true);
			}
		}
		else if (IsKeyTrigger(2, Input::Down))
		{
			auto SE = m_pSEs->GetSEList();
			SE["Select"]->Play();
			for (int i = 0; i < m_pTeams2.size(); i++)
			{
				m_pTeams2[i]->Move(false);
			}
		}
		else if (IsKeyTrigger(2, Input::B))
		{
			auto SE = m_pSEs->GetSEList();
			SE["Decision"]->Play();
			if (!m_bEnd)
			{
				m_bEnd = true;
				FadeOut([]()
					{
						ChangeScene(new CSceneTitle());
						FadeIn(nullptr);
					});
			}
		}
		else if (IsKeyTrigger(2, Input::A))
		{
			bool isMove = false;
			int nSelectTeam = -1;
			auto SE = m_pSEs->GetSEList();
			SE["Decision"]->Play();
			for (int i = 0; i < m_pTeams2.size(); i++)
			{
				isMove = m_pTeams2[i]->GetIsMove();
				if (isMove) break;
				if (m_pTeams2[i]->GetIsSelect()) nSelectTeam = i;
			}

			if (!isMove)
			{
				m_eTeam2 = (TeamKind)nSelectTeam;
				m_bSelectedPlayer2 = true;
			}
		}
	}

	if (m_bSelectedPlayer1 && m_bSelectedPlayer2)
	{
		if (!m_bEnd)
		{
			m_bEnd = true;
			FadeOut([]()
				{
					ChangeScene(new CSceneMemberselect(m_eTeam1, m_eTeam2));
					FadeIn(nullptr);
				});
		}
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

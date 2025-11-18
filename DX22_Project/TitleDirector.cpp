#include "TitleDirector.h"
#include "Main.h"
#include "Input.h"
#include "TitleBackGround.h"
#include "TitleBall.h"
#include "TitleBat.h"
#include "TitleStartButton.h"
#include "TitleEndButton.h"
#include "TitleSelectCursor.h"
#include "TitleLogo.h"
#include "SceneTeamselect.h"

constexpr int ce_nMaxSelectIndex = 1;

CTitleDirector::CTitleDirector()
	: m_ePhase(TitlePhase::Animation), m_nSelect(0)
	, m_pBackGround(nullptr), m_pBall(nullptr)
	, m_pBat{nullptr}, m_pLogo(nullptr)
	, m_pStartButton(nullptr), m_pEndButton(nullptr)
	, m_pSelectCursor(nullptr),  m_bEnd(false)
{

}

CTitleDirector::~CTitleDirector()
{

}

void CTitleDirector::Init()
{
	CScene* pScene = GetScene();

	m_pBackGround = pScene->AddGameObject<CTitleBackGround>("BackGround", Tag::UI);
	m_pBall = pScene->AddGameObject<CTitleBall>("Ball", Tag::UI);
	for (auto itr : m_pBat)
	{
		itr = pScene->AddGameObject<CTitleBat>("Bat", Tag::UI);
	}
	m_pBat[1]->SetIsRight(true);
	m_pStartButton = pScene->AddGameObject<CTitleStartButton>("StartButton", Tag::UI);
	m_pEndButton = pScene->AddGameObject<CTitleEndButton>("EndButton", Tag::UI);
	m_pSelectCursor = pScene->AddGameObject<CTitleSelectCursor>("SelectCursor", Tag::UI);
	m_pLogo = pScene->AddGameObject<CTitleLogo>("Logo", Tag::UI);
}

void CTitleDirector::Update()
{
	if (m_bEnd) return;

	switch (m_ePhase)
	{
	case CTitleDirector::Animation: UpdateAnimation(); break;
	case CTitleDirector::Select: UpdateSelect(); break;
	default: break;
	}
}

void CTitleDirector::UpdateAnimation()
{
	if (IsKeyTrigger(1, Input::A) || IsKeyTrigger(2, Input::A))
	{
		m_ePhase = TitlePhase::Select;

		auto AnimationObjects = GetScene()->GetSameGameObject<CAnimationObject>();
		for (auto itr : AnimationObjects)
		{
			itr->EndAnimation();
		}
		m_pSelectCursor->SetActive(true);
	}

	// ‹^Ž—ƒRƒ‹[ƒ`ƒ“ˆ—...
	// ‰˜‚¢‚Ì‚Å•Ï‚¦‚½‚¢
	m_pBall->StartAnimation();
	if (m_pBall->IsCompliteAnimation())
	{
		m_pBat[0]->StartAnimation();
		m_pBat[1]->StartAnimation();
		if (m_pBat[0]->IsCompliteAnimation() && m_pBat[1]->IsCompliteAnimation())
		{
			m_pLogo->StartAnimation();
			if (m_pLogo->IsCompliteAnimation())
			{
				m_pStartButton->StartAnimation();
				m_pEndButton->StartAnimation();

				if (m_pStartButton->IsCompliteAnimation() && m_pEndButton->IsCompliteAnimation())
				{
					auto AnimationObjects = GetScene()->GetSameGameObject<CAnimationObject>();
					for (auto itr : AnimationObjects)
					{
						itr->EndAnimation();
					}
					m_pSelectCursor->SetActive(true);
					m_ePhase = TitlePhase::Select;
				}
			}
		}
	}
}

void CTitleDirector::UpdateSelect()
{
	if (IsKeyTrigger(1, Input::Down) || IsKeyTrigger(2, Input::Down))
	{
		m_nSelect != ce_nMaxSelectIndex ? m_nSelect++ : NULL;
		m_pSelectCursor->SetIndex(m_nSelect);
	}
	else if (IsKeyTrigger(1, Input::Up) || IsKeyTrigger(2, Input::Up)) 
	{
		m_nSelect != 0 ? m_nSelect-- : NULL;
		m_pSelectCursor->SetIndex(m_nSelect);
	}


	if (IsKeyTrigger(1, Input::A) || IsKeyTrigger(2, Input::A))
	{
		m_bEnd = true;
		switch (m_nSelect)
		{
		case 0:
			FadeOut([]()
				{
					ChangeScene(new CSceneTeamSelect());
					FadeIn(nullptr);
				});
			break;
		case 1:
			AppEnd();
			break;
		}
	}
}

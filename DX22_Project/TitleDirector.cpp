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

CTitleDirector::CTitleDirector()
	: m_ePhase(TitlePhase::Animation)
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
	m_pStartButton = pScene->AddGameObject<CTitleStartButton>("StartButton", Tag::UI);
	m_pEndButton = pScene->AddGameObject<CTitleEndButton>("EndButton", Tag::UI);
	m_pSelectCursor = pScene->AddGameObject<CTitleSelectCursor>("SelectCursor", Tag::UI);
	m_pLogo = pScene->AddGameObject<CTitleLogo>("Logo", Tag::UI);
}

void CTitleDirector::Update()
{
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
	}
}

void CTitleDirector::UpdateSelect()
{

}
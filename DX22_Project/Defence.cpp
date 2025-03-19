#include "Defence.h"
#include "Camera.h"
#include "StrikeZone.h"
#include "SceneGame.h"

CDefence::CDefence()
	: m_pPitchingCursor(nullptr)
	, m_pPitching(nullptr)
{
	m_pPitchingCursor = std::make_unique<CPitchingCursor>();
	m_pPitching = std::make_unique<CPitching>();

	CStrikeZone* pStrikeZone = CStrikeZone::GetInstance().get();
	m_pPitchingCursor->SetStrikeZone(pStrikeZone);
	m_pPitching->SetStrikeZone(pStrikeZone);
	m_pPitching->SetCursor(m_pPitchingCursor.get());
	CBall* pBall = CBall::GetInstance().get();
	pBall->SetCamera(CCamera::GetInstance(CSceneGame::GetCameraKind()).get());
	pBall->SetPitching(m_pPitching.get());
	pBall->SetPitchingCursor(m_pPitchingCursor.get());
}


CDefence::~CDefence()
{

}

void CDefence::Update()
{
	m_pPitchingCursor->Update();
	m_pPitching->Update();
}

void CDefence::Draw()
{
	m_pPitchingCursor->Draw();
	m_pPitching->Draw();
}

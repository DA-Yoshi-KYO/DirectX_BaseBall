#include "Defence.h"
#include "Camera.h"
#include "StrikeZone.h"
#include "SceneGame.h"
#include "Ball.h"

CDefence::CDefence()
	: m_pPitchingCursor(nullptr)
	, m_pPitching(nullptr)
	, m_pFielding(nullptr)
{
	m_pPitchingCursor = std::make_unique<CPitchingCursor>();
	m_pPitching = std::make_unique<CPitching>();
	m_pFielding = std::make_unique<CFielding>();

	CStrikeZone* pStrikeZone = CStrikeZone::GetInstance().get();
	m_pPitchingCursor->SetStrikeZone(pStrikeZone);
	m_pPitching->SetStrikeZone(pStrikeZone);
	m_pPitching->SetCursor(m_pPitchingCursor.get());
	CBall* pBall = CBall::GetInstance().get();
	pBall->SetPitching(m_pPitching.get());
	pBall->SetPitchingCursor(m_pPitchingCursor.get());
}


CDefence::~CDefence()
{

}

void CDefence::Update()
{
	if (CBall::GetInstance()->GetPhase() == CBall::BallPhase::Batting)
	{
		m_pPitchingCursor->Update();
		m_pPitching->Update();
	}
	m_pFielding->Update();
}

void CDefence::Draw()
{
	if (CBall::GetInstance()->GetPhase() == CBall::BallPhase::Batting)
	{
		m_pPitchingCursor->Draw();
		m_pPitching->Draw();
	}
	m_pFielding->Draw();
}

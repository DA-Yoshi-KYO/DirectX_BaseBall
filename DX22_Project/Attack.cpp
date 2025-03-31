#include "Attack.h"
#include "Ball.h"
#include "StrikeZone.h"

CAttack::CAttack()
	: m_pBattingCursor(nullptr)
	, m_pBatting(nullptr)
	, m_pRunning(nullptr)
{
	m_pBattingCursor = std::make_unique<CBattingCursor>();
	m_pBatting = std::make_unique<CBatting>();
	m_pRunning = std::make_unique<CRunning>();

	m_pBattingCursor->SetStrikeZone(CStrikeZone::GetInstance().get());
	m_pBatting->SetCursor(m_pBattingCursor.get());
	CBall* pBall = CBall::GetInstance().get();
	m_pBatting->SetBall(pBall);
	pBall->SetBatting(m_pBatting.get());
}

CAttack::~CAttack()
{
}

void CAttack::Update()
{
	if (CBall::GetInstance()->GetPhase() == CBall::BallPhase::Batting)
	{
		m_pBattingCursor->Update();
		m_pBatting->Update();
	}
	m_pRunning->Update();
}

void CAttack::Draw()
{
	if (CBall::GetInstance()->GetPhase() == CBall::BallPhase::Batting)
	{
		m_pBattingCursor->Draw();
		m_pBatting->Draw();
	}
	m_pRunning->Draw();
}

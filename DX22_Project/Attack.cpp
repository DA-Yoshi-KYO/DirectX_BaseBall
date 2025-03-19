#include "Attack.h"
#include "Ball.h"
#include "StrikeZone.h"

CAttack::CAttack()
	: m_pBattingCursor(nullptr)
{
	m_pBattingCursor = std::make_unique<CBattingCursor>();
	m_pBatting = std::make_unique<CBatting>();

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
	m_pBattingCursor->Update();
	m_pBatting->Update();
}

void CAttack::Draw()
{
	m_pBattingCursor->Draw();
	m_pBatting->Draw();
}

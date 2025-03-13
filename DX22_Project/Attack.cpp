#include "Attack.h"

CAttack::CAttack()
{
	m_pStrikeZone = std::make_unique<CStrikeZone>();
}

CAttack::~CAttack()
{
}

void CAttack::Update()
{
	m_pStrikeZone->Update();
}

void CAttack::Draw()
{
	m_pStrikeZone->Draw();
}

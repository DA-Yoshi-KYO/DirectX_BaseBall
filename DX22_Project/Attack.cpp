#include "Attack.h"
#include "SceneGame.h"

CAttack::CAttack()
	: m_pStrikeZone(nullptr), m_pCursor(nullptr)
{
	m_pStrikeZone = std::make_unique<CStrikeZone>();
	m_pCursor = std::make_unique<CCursor>();

	m_pCursor->SetStrikeZone(m_pStrikeZone.get());
}

CAttack::~CAttack()
{
}

void CAttack::Update()
{
	m_pStrikeZone->Update();
	m_pCursor->Update((int)CSceneGame::Playing::Attack);
}

void CAttack::Draw()
{
	m_pStrikeZone->Draw();
	m_pCursor->Draw((int)CSceneGame::Playing::Attack);
}

#include "Defence.h"
#include "SceneGame.h"

CDefence::CDefence()
	: m_pStrikeZone(nullptr), m_pCursor(nullptr), m_pPitching(nullptr)
{
	m_pStrikeZone = std::make_unique<CStrikeZone>();
	m_pCursor = std::make_unique<CCursor>();
	m_pPitching = std::make_unique<CPitching>();

	m_pCursor->SetStrikeZone(m_pStrikeZone.get());

	m_pPitching->SetStrikeZone(m_pStrikeZone.get());
	m_pPitching->SetCursor(m_pCursor.get());
}


CDefence::~CDefence()
{

}

void CDefence::Update()
{
	m_pStrikeZone->Update();
	m_pCursor->Update((int)CSceneGame::Playing::Defence);
	m_pPitching->Update();
}

void CDefence::Draw()
{
	m_pStrikeZone->Draw();
	m_pCursor->Draw((int)CSceneGame::Playing::Defence);
	m_pPitching->Draw();
}

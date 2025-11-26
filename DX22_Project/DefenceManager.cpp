#include "DefenceManager.h"
#include "Input.h"
#include "TeamManager.h"
#include "Main.h"

CDefenceManager::CDefenceManager()
	: m_nPlayerNo(2), m_pPitching(nullptr)
{

}

CDefenceManager::~CDefenceManager()
{

}

void CDefenceManager::Init()
{
	CScene* pScene = GetScene();

	pScene->AddGameObject<CPitchingCursor>("PitchingCursor", Tag::UI);

	m_pPitching = std::make_unique<CPitching>();
	m_pPitching->Init();
}

void CDefenceManager::Update()
{
	m_pPitching->Update(m_nPlayerNo);
}

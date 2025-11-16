#include "AttackManager.h"
#include "Main.h"
#include "BattingCursor.h"

CAttackManager::CAttackManager()
	: m_nPlayerNo(1)
{

}

CAttackManager::~CAttackManager()
{

}

void CAttackManager::Init()
{
	CScene* pScene = GetScene();

	pScene->AddGameObject<CBattingCursor>("BattingCursor", Tag::UI);

	m_pBatting = std::make_unique<CBatting>();
}

void CAttackManager::Update()
{
	m_pBatting->Update(m_nPlayerNo);
}

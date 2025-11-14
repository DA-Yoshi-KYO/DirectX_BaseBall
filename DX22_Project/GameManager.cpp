#include "BallCount.h"
#include "Camera.h"
#include "Sprite.h"
#include "Main.h"
#include "TeamManager.h"
#include "GameManager.h"

CGameManager* CGameManager::m_pInstance = nullptr;

CGameManager::CGameManager()
	: m_pCountManager(nullptr), m_pAttackManager(nullptr)
{

}

CGameManager::~CGameManager()
{

}

void CGameManager::Init()
{
	if (!m_pCountManager) m_pCountManager = new CCountManager();
	if (!m_pAttackManager) m_pAttackManager = new CAttackManager();
}

void CGameManager::Update()
{
	//if (m_pAttackManager) m_pAttackManager->Update();
	if (m_pCountManager) m_pCountManager->Update();
}

void CGameManager::Draw()
{
	
}

CGameManager* CGameManager::GetInstance()
{ 
	if (!m_pInstance)
	{
		m_pInstance = new CGameManager();
	}

	return m_pInstance;
}

#include "BallCount.h"
#include "Camera.h"
#include "Sprite.h"
#include "Main.h"
#include "TeamManager.h"
#include "GameManager.h"

CGameManager* CGameManager::m_pInstance = nullptr;

CGameManager::CGameManager()
    : m_pCountManager(nullptr), m_pAttackManager(nullptr)
    , m_pDefenceManager(nullptr), m_pTeamManager{ nullptr, nullptr }
    , m_ePhase(GamePhase::Batting)
{

}

CGameManager::~CGameManager()
{

}

void CGameManager::Init()
{
    if (!m_pCountManager)
    {
        m_pCountManager = std::make_unique<CCountManager>();
        m_pCountManager->Init();
    }
    if (!m_pAttackManager)
    {
        m_pAttackManager = std::make_unique<CAttackManager>();
        m_pAttackManager->Init();
    }
    if (!m_pDefenceManager)
    {
        m_pDefenceManager = std::make_unique<CDefenceManager>();
        m_pDefenceManager->Init();
    }
    if (!m_pFieldManager)
    {
        m_pFieldManager = std::make_unique<CFieldManager>();
        m_pFieldManager->Init();
    }
    for (int i = 0; i < 2; i++)
    {
        if (!m_pTeamManager[i])
        {
            m_pTeamManager[i] = std::make_unique<CTeamDirector>(i + 1);
        }
    }
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

void CGameManager::Release()
{
    SAFE_DELETE(m_pInstance);
}
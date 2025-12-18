#include "GameManager.h"
#include "Main.h"
#include "Runner.h"

CGameManager* CGameManager::m_pInstance = nullptr;
constexpr float ce_fWaitMaxTime = 3.0f;

CGameManager::CGameManager()
    : m_pCountDirecter(nullptr), m_pAttackDirecter(nullptr)
    , m_pDefenceDirecter(nullptr), m_pTeamDirecter{ nullptr, nullptr }
    , m_ePhase(GamePhase::Batting), m_fWaitTime(0.0f)
{
    if (!m_pFieldDirecter)
    {
        m_pFieldDirecter = std::make_unique<CFieldDirecter>();
    }
    if (!m_pCountDirecter)
    {
        m_pCountDirecter = std::make_unique<CCountDirecter>();
    }
    if (!m_pAttackDirecter)
    {
        m_pAttackDirecter = std::make_unique<CAttackDirecter>();
    }
    if (!m_pDefenceDirecter)
    {
        m_pDefenceDirecter = std::make_unique<CDefenceDirecter>();
    }

    for (int i = 0; i < 2; i++)
    {
        if (!m_pTeamDirecter[i])
        {
            m_pTeamDirecter[i] = std::make_unique<CTeamDirector>(i + 1);
        }
    }
}

CGameManager::~CGameManager()
{

}

void CGameManager::Init()
{
    m_pFieldDirecter->Init();
    m_pCountDirecter->Init();
    m_pAttackDirecter->Init();
    m_pDefenceDirecter->Init();
}

void CGameManager::Update()
{
    CCamera* pCamera = CCamera::GetInstance();
    switch (m_ePhase)
    {
    case GamePhase::Batting:
        pCamera->SetCameraKind(CAM_BATTER);
        break;
    case GamePhase::InPlay:
        pCamera->SetCameraKind(CAM_INPLAY);
        CheckEndInplay();
        break;
    default:
        break;
    }
    if (m_pAttackDirecter) m_pAttackDirecter->Update();
    if (m_pDefenceDirecter) m_pDefenceDirecter->Update();
    if (m_pCountDirecter) m_pCountDirecter->Update();
}

void CGameManager::Draw()
{
    
}

void CGameManager::EndAllInplay()
{
    int nPlayerNo = m_pAttackDirecter->GetPlayerNo();
    for (int i = 0; i < 2; i++)
    {
        m_pTeamDirecter[i]->EndInplay(m_pTeamDirecter[i]->GetTeamNo() == nPlayerNo);
    }
    m_pFieldDirecter->EndInplay();
    m_pAttackDirecter->EndInplay();
    m_pDefenceDirecter->EndInplay();
    m_pCountDirecter->EndInplay();
    m_ePhase = GamePhase::Batting;
}

void CGameManager::FaulBall()
{
    int nPlayerNo = m_pAttackDirecter->GetPlayerNo();
    for (int i = 0; i < 2; i++)
    {
        m_pTeamDirecter[i]->EndInplay(m_pTeamDirecter[i]->GetTeamNo() == nPlayerNo);
    }
    m_pFieldDirecter->EndInplay();
    CRunner* pRunner = m_pAttackDirecter->GetRunning()->GetBatterRunner();
    if (pRunner) pRunner->Destroy();
    m_pAttackDirecter->EndInplay();
    m_pDefenceDirecter->EndInplay();
    m_pCountDirecter->Faul();
    m_ePhase = GamePhase::Batting;
}

void CGameManager::HomeRun()
{
    auto RunnerList = m_pAttackDirecter->GetRunning()->GetAllRunner();

    for (auto itr : RunnerList)
    {
        m_pCountDirecter->AddScore();
        itr->Destroy();
    }

    EndAllInplay();
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

void CGameManager::CheckEndInplay()
{
    CScene* pScene = GetScene();
    
    CBall* pBall = pScene->GetGameObject<CBall>();
    if (!pBall->GetCaught()) return;

    auto RunnerList = pScene->GetSameGameObject<CRunner>();
    for (auto itr : RunnerList)
    {
        GotBase eKind = itr->GetTouchBase();

        switch (eKind)
        {
        case GotBase::None:
            m_fWaitTime = 0.0f;
            return;
            break;
        }
    }

    m_fWaitTime += 1.0f / fFPS;

    if (m_fWaitTime >= ce_fWaitMaxTime)
    {
        m_fWaitTime = 0.0f;
        EndAllInplay();
    }
}

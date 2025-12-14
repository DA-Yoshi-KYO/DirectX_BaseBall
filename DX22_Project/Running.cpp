#include "Running.h"
#include "Main.h"

CRunning::CRunning()
	: m_pBatterRunner(nullptr), m_pRunners{}
{

}

CRunning::~CRunning()
{

}

void CRunning::Update(int AttackPlayer)
{

}

void CRunning::Draw()
{

}

void CRunning::SetBatterRunner(Quality speed)
{
	CRunner* pRunner = GetScene()->AddGameObject<CRunner>("Runner", Tag::GameObject);
	pRunner->SetRunnerSpeed(speed);
	pRunner->SetStatus(RunnerStatus::ToNext);
	m_pAllRunners.push_back(pRunner);
}

void CRunning::AddFirstRunner(Quality speed)
{
	CRunner* pRunner = GetScene()->AddGameObject<CRunner>("Runner", Tag::GameObject);
	pRunner->SetRunnerSpeed(speed);
	m_pAllRunners.push_back(pRunner);
	m_pRunners.push_back(pRunner);
}

void CRunning::RemoveRunner(CRunner* pThis)
{
	auto findRunnersItr = std::find(m_pRunners.begin(), m_pRunners.end(), pThis);
	if (findRunnersItr != m_pRunners.end()) m_pRunners.erase(findRunnersItr);

	auto findAllRunnersItr = std::find(m_pAllRunners.begin(), m_pAllRunners.end(), pThis);
	if (findAllRunnersItr != m_pAllRunners.end()) m_pAllRunners.erase(findAllRunnersItr);

	if (m_pBatterRunner == pThis)  m_pBatterRunner = nullptr;
}

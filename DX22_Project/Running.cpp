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
}

void CRunning::AddFirstRunner(Quality speed)
{
	CRunner* pRunner = GetScene()->AddGameObject<CRunner>("Runner", Tag::GameObject);
	pRunner->SetRunnerSpeed(speed);
}

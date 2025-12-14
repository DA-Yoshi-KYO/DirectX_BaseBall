#include "AttackDirecter.h"
#include "Main.h"
#include "BattingCursor.h"
#include "Runner.h"

CAttackDirecter::CAttackDirecter()
	: m_nPlayerNo(1)
{

}

CAttackDirecter::~CAttackDirecter()
{

}

void CAttackDirecter::Init()
{
	CScene* pScene = GetScene();

	pScene->AddGameObject<CBattingCursor>("BattingCursor", Tag::UI);

	m_pBatting = std::make_unique<CBatting>();
	m_pRunning = std::make_unique<CRunning>();
}

void CAttackDirecter::Update()
{
	m_pBatting->Update(m_nPlayerNo);
	m_pRunning->Update(m_nPlayerNo);
}

void CAttackDirecter::EndInplay()
{
	CScene* pScene = GetScene();
	auto RunnerList = pScene->GetSameGameObject<CRunner>();
	for (auto itr : RunnerList)
	{
		itr->ToNormalRunner();
		itr->ResetPos();
	}
}

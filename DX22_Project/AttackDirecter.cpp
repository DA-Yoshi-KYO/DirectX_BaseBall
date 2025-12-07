#include "AttackDirecter.h"
#include "Main.h"
#include "BattingCursor.h"

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
}

void CAttackDirecter::Update()
{
	m_pBatting->Update(m_nPlayerNo);
}

void CAttackDirecter::EndInplay()
{
}

#include "DefenceDirecter.h"
#include "Input.h"
#include "TeamDirecter.h"
#include "Main.h"

CDefenceDirecter::CDefenceDirecter()
	: m_nPlayerNo(2), m_pPitching(nullptr)
{

}

CDefenceDirecter::~CDefenceDirecter()
{

}

void CDefenceDirecter::Init()
{
	CScene* pScene = GetScene();

	pScene->AddGameObject<CPitchingCursor>("PitchingCursor", Tag::UI);

	m_pPitching = std::make_unique<CPitching>();
	m_pPitching->Init();
	m_pFielding = std::make_unique<CFielding>();
	m_pFielding->Init();
	m_pFielding->SetFielderData(m_nPlayerNo);
}

void CDefenceDirecter::Update()
{
	m_pPitching->Update(m_nPlayerNo);
	m_pFielding->Update(m_nPlayerNo);
}

void CDefenceDirecter::EndInplay()
{
	m_pFielding->ResetFieldersPos();
}

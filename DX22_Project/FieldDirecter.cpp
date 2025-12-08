#include "FieldDirecter.h"
#include "Main.h"


CFieldDirecter::CFieldDirecter()
{
}

CFieldDirecter::~CFieldDirecter()
{
}

void CFieldDirecter::Init()
{
	CScene* pScene = GetScene();

	m_pBall = pScene->AddGameObject<CBall>("Ball", Tag::GameObject);
	m_pField = pScene->AddGameObject<CField>("Field", Tag::Field);
	m_pStrikeZone = pScene->AddGameObject<CStrikeZone>("StrikeZone", Tag::UI);

	for (int i = 0; i < (int)BaseKind::Max; i++)
	{
		m_pBase[i] = pScene->AddGameObject<CBase>("Base", Tag::GameObject);
		m_pBase[i]->Init(BaseKind(i));
	}

}

void CFieldDirecter::EndInplay()
{
	m_pBall->SetCaught(false);
	m_pBall->SetIsFryBall(true);
	m_pBall->SetIsFryChatch(false);
}

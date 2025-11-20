#include "MemberIcon.h"
#include "Main.h"

CMemberIcon::CMemberIcon()
	: m_pBack(nullptr), m_pName(nullptr)
{

}

CMemberIcon::~CMemberIcon()
{

}

void CMemberIcon::Init(CPlayerDataBase* pData)
{
	m_pPlayerData = pData;
	CScene* pScene = GetScene();
	PlayerData data = m_pPlayerData->GetPlayerData();

	m_pBack = pScene->AddGameObject<CMemberBack>("MemberBack", Tag::UI);
	m_pBack->Init(data.m_eMainPosition);
	m_pName = pScene->AddGameObject<CMemberName>("MemberName", Tag::UI);
	m_pName->Init(data.m_wsName);
}

void CMemberIcon::SetPos(DirectX::XMFLOAT3 pos)
{
	m_pBack->SetPos(pos);
	m_pName->SetPos(pos);
}
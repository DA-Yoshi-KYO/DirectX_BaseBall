#include "MemberIcon.h"

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
	m_pPlayerData->GetPlayerData().m_eMainPosition;
	m_pPlayerData->GetPlayerData().m_wsName;
}

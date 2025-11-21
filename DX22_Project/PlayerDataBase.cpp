#include "PlayerDataBase.h"

CPlayerDataBase::CPlayerDataBase()
{
	m_tPlayerData.m_wsName = L"";
	m_tPlayerData.m_eHandy = Hand::Right;
	m_tPlayerData.m_nLineupNo = 0;
	m_tPlayerData.m_eMainPosition = Positions::None;
	m_tPlayerData.m_eEntryPosition = Positions::None;
}

CPlayerDataBase::~CPlayerDataBase()
{

}

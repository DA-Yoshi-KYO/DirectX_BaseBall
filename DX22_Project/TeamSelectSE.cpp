#include "TeamSelectSE.h"

CTeamSelectSE::CTeamSelectSE()
	: CGameObject()
{

}

CTeamSelectSE::~CTeamSelectSE()
{

}

void CTeamSelectSE::Init()
{
	m_pSEList["Select"] = AddComponent<CAudio>();
	m_pSEList["Select"]->Load(PATH_SE("Select.wav"));

	m_pSEList["Decision"] = AddComponent<CAudio>();
	m_pSEList["Decision"]->Load(PATH_SE("Decision.wav"));
}

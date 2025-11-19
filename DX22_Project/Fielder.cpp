#include "Fielder.h"

CFielder::CFielder()
{
	m_tFielderData.m_nTrajectory = 2;
	m_tFielderData.m_eMeet = Quality::D;
	m_tFielderData.m_ePower = Quality::D;
	m_tFielderData.m_eSpeed = Quality::D;
	m_tFielderData.m_eThrowing = Quality::D;
	m_tFielderData.m_eDefence = Quality::D;
	m_tFielderData.m_eChatch = Quality::D;
}

CFielder::~CFielder()
{

}

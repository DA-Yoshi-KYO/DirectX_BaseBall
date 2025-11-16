#include "DefenceManager.h"
#include "Input.h"
#include "TeamManager.h"

CDefenceManager::CDefenceManager()
	: m_nPlayerNo(2), m_pPitching(nullptr)
{

}

CDefenceManager::~CDefenceManager()
{

}

void CDefenceManager::Init()
{
	m_pPitching = std::make_unique<CPitching>();
}

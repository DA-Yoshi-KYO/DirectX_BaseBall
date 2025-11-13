#include "CountManager.h"
#include "Main.h"
#include "StrikeCount.h"
#include "BallCount.h"
#include "OutCount.h"
#include "BaseCount.h"
#include "Score.h"
#include "Inning.h"

CCountManager::CCountManager()
	: m_pStrikeCount{}, m_pBallCount{}, m_pOutCount{}, m_pBaseCount{}
	, m_pScore{}, m_pInning(nullptr)
{
	m_tParam.m_nStrikeCount = 0;
	m_tParam.m_nBallCount = 0;
	m_tParam.m_nOutCount = 0;
}

CCountManager::~CCountManager()
{

}

void CCountManager::Init()
{
	CScene* pScene = GetScene();

	for (int i = 0; i < m_pStrikeCount.size(); i++)
	{
		m_pStrikeCount[i] = pScene->AddGameObject<CStrikeCount>("StrikeCount", Tag::UI);
	}

	for (int i = 0; i < m_pBallCount.size(); i++)
	{
		m_pBallCount[i] = pScene->AddGameObject<CBallCount>("BallCount", Tag::UI);
	}

	for (int i = 0; i < m_pOutCount.size(); i++)
	{
		m_pOutCount[i] = pScene->AddGameObject<COutCount>("OutCount", Tag::UI);
	}

	for (int i = 0; i < m_pOutCount.size(); i++)
	{
		m_pScore[i] = pScene->AddGameObject<CScore>("Score", Tag::UI);
	}

	m_pInning = pScene->AddGameObject<CInning>("Inning", Tag::UI);
}

void CCountManager::Update()
{
	for (int i = 0; i < m_pStrikeCount.size(); i++)
	{
		if (i < m_tParam.m_nStrikeCount) m_pStrikeCount[i]->SetActive(true);
		else m_pStrikeCount[i]->SetActive(false);
	}

	for (int i = 0; i < m_pBallCount.size(); i++)
	{
		if (i < m_tParam.m_nBallCount) m_pBallCount[i]->SetActive(true);
		else m_pBallCount[i]->SetActive(false);
	}

	for (int i = 0; i < m_pOutCount.size(); i++)
	{
		if (i < m_tParam.m_nOutCount) m_pOutCount[i]->SetActive(true);
		else m_pOutCount[i]->SetActive(false);
	}
}

void CCountManager::AddStrikeCount()
{
	m_tParam.m_nStrikeCount++;
	if (m_tParam.m_nStrikeCount >= 3)
	{
		ThreeStrike();
	}
}

void CCountManager::AddBallCount()
{
	m_tParam.m_nBallCount++;
	if (m_tParam.m_nBallCount >= 4)
	{
		FourBall();
	}
}

void CCountManager::AddOutCount()
{
	m_tParam.m_nOutCount++;
	m_tParam.m_nStrikeCount = 0;
	m_tParam.m_nBallCount = 0;
	if (m_tParam.m_nOutCount >= 3)
	{
		ThreeOut();
	}
}

void CCountManager::SetIsBase(bool isBase, int index)
{
	if (index < 0 || index >= 3) return;
	
	m_pBaseCount[index]->SetActive(isBase);
}

void CCountManager::ThreeStrike()
{
	AddOutCount();
}

void CCountManager::FourBall()
{
	m_tParam.m_nStrikeCount = 0;
	m_tParam.m_nBallCount = 0;

	for (int i = 0; i < m_pBaseCount.size(); i++)
	{
		if (!m_pBaseCount[i]->GetActive())
		{
			m_pBaseCount[i]->SetActive(true);
			break;
		}
		else
		{
			// TODO: âüÇµèoÇµÇÃèàóùÇãLèq
			if (i == m_pBaseCount.size() - 1)
			{

			}
		}
	}
}

void CCountManager::ThreeOut()
{
	m_tParam.m_nOutCount = 0;
	for (int i = 0; i < m_pBaseCount.size(); i++)
	{
		m_pBaseCount[i]->SetActive(false);
	}
	m_pInning->InningProgress();
}

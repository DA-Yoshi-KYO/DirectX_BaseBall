#include "CountManager.h"
#include "Main.h"
#include "StrikeCount.h"
#include "BallCount.h"
#include "OutCount.h"
#include "BaseCount.h"
#include "Score.h"
#include "Inning.h"

constexpr DirectX::XMFLOAT3 ce_fCountPos = { 395.0f,271.0f,0.0f };
constexpr DirectX::XMFLOAT3 ce_fCountSize = { 35.0f,35.0f,0.0f };
constexpr float ce_fCountAjustY = 37.3f;

constexpr DirectX::XMFLOAT3 ce_fBasePos = { 515.0f,210.0f,0.0f };
constexpr DirectX::XMFLOAT3 ce_fBaseSize = { 50.0f,40.0f,0.0f };
constexpr DirectX::XMFLOAT3 ce_fBaseAjust = { 42.0f,40.0f,0.0f };

constexpr DirectX::XMFLOAT3 ce_fScoreTopPos = { 500.0f,320.0f,0.0f };
constexpr DirectX::XMFLOAT3 ce_fScoreBottomPos = { 560.0f,320.0f,0.0f };

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
		m_pStrikeCount[i]->SetPos({ce_fCountSize.x * i + ce_fCountPos.x,-ce_fCountAjustY + ce_fCountPos.y,0.0f });
		m_pStrikeCount[i]->SetSize(ce_fCountSize);
	}

	for (int i = 0; i < m_pBallCount.size(); i++)
	{
		m_pBallCount[i] = pScene->AddGameObject<CBallCount>("BallCount", Tag::UI);
		m_pBallCount[i]->SetPos({ ce_fCountSize.x * i + ce_fCountPos.x,0.0f + ce_fCountPos.y,0.0f });
		m_pBallCount[i]->SetSize(ce_fCountSize);
	}

	for (int i = 0; i < m_pOutCount.size(); i++)
	{
		m_pOutCount[i] = pScene->AddGameObject<COutCount>("OutCount", Tag::UI);
		m_pOutCount[i]->SetPos({ ce_fCountSize.x * i + ce_fCountPos.x,-ce_fCountAjustY * 2.0f + ce_fCountPos.y,0.0f });
		m_pOutCount[i]->SetSize(ce_fCountSize);
	}

	for (int i = 0; i < m_pBaseCount.size(); i++)
	{
		m_pBaseCount[i] = pScene->AddGameObject<CBaseCount>("BaseCount", Tag::UI);
		m_pBaseCount[i]->SetPos({ ce_fCountSize.x * i + ce_fCountPos.x,-ce_fCountAjustY * 2.0f + ce_fCountPos.y,0.0f });
		m_pBaseCount[i]->SetSize(ce_fCountSize);
		switch (i)
		{
		case 0:
			m_pBaseCount[i]->SetPos({ ce_fBasePos.x + ce_fBaseAjust.x * 2.0f,ce_fBasePos.y,0.0f });
			break;
		case 1:
			m_pBaseCount[i]->SetPos({ ce_fBasePos.x + ce_fBaseAjust.x,ce_fBasePos.y + ce_fBaseAjust.y,0.0f });
			break;
		case 2:
			m_pBaseCount[i]->SetPos(ce_fBasePos);
			break;
		default:
			break;
		}
	}

	for (int i = 0; i < m_pOutCount.size(); i++)
	{
		m_pScore[i] = pScene->AddGameObject<CScore>("Score", Tag::UI);
	}

	m_pScore[0]->SetScoreOnePos(ce_fScoreTopPos);
	m_pScore[1]->SetScoreOnePos(ce_fScoreBottomPos);

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

#include "SelectTeam.h"
#include "SpriteRenderer.h"
#include "Oparation.h"

constexpr DirectX::XMFLOAT3 ce_f3SelectClothesPos = DirectX::XMFLOAT3(-400.0f,0.0f,0.0f);
constexpr float ce_fMovePos = 300.0f;
constexpr float ce_fPartitionOffset = 150.0f;
constexpr float ce_fMoveDuration = 1.0f;

CSelectTeam::CSelectTeam()
	: m_nIndex(0), m_bIsMove(false)
	, m_f3InitPos(), m_f3TargetPos(), m_fTime(0.0f)
{
	m_tParam.m_f3Size = DirectX::XMFLOAT3(100.0f, 100.0f, 0.0f);
}

CSelectTeam::~CSelectTeam()
{

}

void CSelectTeam::Init(CTeamManager::Teams kind, int TeamNo)
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();

	m_tParam.m_f3Pos = DirectX::XMFLOAT3(-400.0f + 800.0f * (TeamNo - 1), 0.0f, 0.0f);
	switch (kind)
	{
	case CTeamManager::Bears:
		m_nIndex = CTeamManager::Teams::Max / 2;
		pRenderer->Load(PATH_TEX("Teams/Bears.png"));
		break;
	case CTeamManager::Rabbits:
		m_nIndex = CTeamManager::Teams::Max / 2 + 1;
		pRenderer->Load(PATH_TEX("Teams/Rabbits.png"));
		break;
	case CTeamManager::Tigers:
		m_nIndex = CTeamManager::Teams::Max / 2 + 2;
		pRenderer->Load(PATH_TEX("Teams/Tigers.png"));
		break;
	case CTeamManager::Elephants:
		m_nIndex = CTeamManager::Teams::Max / 2 - 2;
		pRenderer->Load(PATH_TEX("Teams/Elephants.png"));
		break;
	case CTeamManager::Monkeys:
		m_nIndex = CTeamManager::Teams::Max / 2 - 1;
		pRenderer->Load(PATH_TEX("Teams/Monkeys.png"));
		break;
	default:
		break;
	}

	m_tParam.m_f3Pos.y = ce_fMovePos * CTeamManager::Teams::Max / 2 - m_nIndex;
}

void CSelectTeam::Update()
{
	if (m_bIsMove)
	{
		m_fTime += 1.0f / fFPS;
		m_tParam.m_f3Pos = m_f3InitPos + (m_f3TargetPos - m_f3InitPos) * (m_fTime / ce_fMoveDuration);
		m_bIsMove = m_fTime <= ce_fMoveDuration;
	}
	else m_fTime = 0.0f;
}

void CSelectTeam::Move(bool isUp)
{
	if (m_bIsMove) return;
	m_bIsMove = true;

	m_f3InitPos = m_tParam.m_f3Pos;
	if (isUp)
	{
		if (m_nIndex == CTeamManager::Teams::Max - 1)
		{
			DirectX::XMFLOAT3 f3MostUp = ce_f3SelectClothesPos;
			f3MostUp.y -= ce_fMovePos * int(CTeamManager::Teams::Max / 2);
			m_f3InitPos = m_f3TargetPos = f3MostUp;
			m_nIndex = 0;
		}
		else
		{
			m_f3TargetPos = m_tParam.m_f3Pos;
			m_f3TargetPos.y += ce_fMovePos;
			m_nIndex++;
		}
	}
	else
	{
		if (m_nIndex == 0)
		{
			DirectX::XMFLOAT3 f3MostDown = ce_f3SelectClothesPos;
			f3MostDown.y += ce_fMovePos * int(CTeamManager::Teams::Max / 2);
			m_f3InitPos = m_f3TargetPos = f3MostDown;
			m_nIndex = CTeamManager::Teams::Max - 1;
		}
		else
		{
			m_f3TargetPos = m_tParam.m_f3Pos;
			m_f3TargetPos.y -= ce_fMovePos;
			m_nIndex--;
		}
	}
}

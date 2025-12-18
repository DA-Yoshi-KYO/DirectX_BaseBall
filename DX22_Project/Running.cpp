#include "Running.h"
#include "Main.h"
#include "Ball.h"
#include "Input.h"

CRunning::CRunning()
	: m_pBatterRunner(nullptr), m_pRunners{}
{

}

CRunning::~CRunning()
{

}

void CRunning::Update(int AttackPlayer)
{
	if (IsKeyPress(AttackPlayer, Input::Y))
	{
		for (auto itr : m_pAllRunners)
		{
			if (itr->GetStatus() != RunnerStatus::CompulsoryToNext) itr->SetStatus(RunnerStatus::ToNext);
		}
	}
	else if (IsKeyPress(AttackPlayer, Input::B))
	{
		for (auto itr : m_pAllRunners)
		{
			if (itr->GetStatus() != RunnerStatus::CompulsoryToNext) itr->SetStatus(RunnerStatus::ToBack);
		}
	}
}

void CRunning::Draw()
{

}

void CRunning::SetBatterRunner(Quality speed)
{
	m_pBatterRunner = GetScene()->AddGameObject<CRunner>("Runner", Tag::GameObject);
	m_pBatterRunner->SetRunnerSpeed(speed);
	m_pBatterRunner->SetStatus(RunnerStatus::CompulsoryToNext);
	m_pAllRunners.push_back(m_pBatterRunner);
}

void CRunning::AddFirstRunner(Quality speed)
{
	CRunner* pRunner = GetScene()->AddGameObject<CRunner>("Runner", Tag::GameObject);
	pRunner->SetFirstBaseRunner();
	pRunner->SetRunnerSpeed(speed);
	m_pAllRunners.push_back(pRunner);
	m_pRunners.push_back(pRunner);
}

void CRunning::ToNormalRunner()
{
	if (m_pBatterRunner)
	{
		if (!m_pBatterRunner->IsDestroy())
		{
			m_pRunners.push_back(m_pBatterRunner);
			m_pBatterRunner = nullptr;
		}
	}
}

void CRunning::RemoveRunner(CRunner* pThis)
{
	auto findRunnersItr = std::find(m_pRunners.begin(), m_pRunners.end(), pThis);
	if (findRunnersItr != m_pRunners.end()) m_pRunners.erase(findRunnersItr);

	auto findAllRunnersItr = std::find(m_pAllRunners.begin(), m_pAllRunners.end(), pThis);
	if (findAllRunnersItr != m_pAllRunners.end()) m_pAllRunners.erase(findAllRunnersItr);

	if (m_pBatterRunner == pThis)  m_pBatterRunner = nullptr;
}

CRunner* CRunning::GetBackRunner(CRunner* pRunner)
{
	auto findItr = std::find(m_pAllRunners.begin(), m_pAllRunners.end(), pRunner);
	if (findItr == m_pAllRunners.end()) return nullptr;
	if (findItr == m_pAllRunners.begin()) return nullptr;

	std::advance(findItr, -1);

	return *findItr;
}

bool CRunning::IsAllBaseTight(BaseKind base)
{
	int nCheck = 1;
	int nCheckNum = 0;
	switch (base)
	{
	case BaseKind::Home:
		nCheck = 3;
		for (auto itr : m_pRunners)
		{
			GotBase oldBase = itr->GetOldBase();
			switch (oldBase)
			{
			case GotBase::First: 
			case GotBase::Second: 
			case GotBase::Third: 
				nCheckNum++;
				break;
			}
		}
		break;
	case BaseKind::First:
		return true;
		break;
	case BaseKind::Second:
		nCheck = 1;
		for (auto itr : m_pRunners)
		{
			GotBase oldBase = itr->GetOldBase();
			switch (oldBase)
			{
			case GotBase::First: 
				nCheckNum++;
				break;
			}
		}
		break;
	case BaseKind::Third:
		nCheck = 2;
		for (auto itr : m_pRunners)
		{
			GotBase oldBase = itr->GetOldBase();
			switch (oldBase)
			{
			case GotBase::First: 
			case GotBase::Second: 
				nCheckNum++;
				break;
			}
		}
		break;
	case BaseKind::Max:
		break;
	default:
		break;
	}

	return nCheck == nCheckNum;
}

bool CRunning::IsTight(CRunner* pThis)
{
	GotBase thisCurrentBase = pThis->GetNowBase();
	GotBase thisTouchBase = pThis->GetTouchBase();
	bool isTight = false;
	if (thisTouchBase != GotBase::None)  isTight = true;

	switch (thisCurrentBase)
	{
	case GotBase::None:
		for (auto itr : m_pAllRunners)
		{
			if (itr == pThis) continue;
			if (itr->GetTouchBase() == GotBase::First) isTight = true;
		}
		break;
	case GotBase::First:
		for (auto itr : m_pAllRunners)
		{
			if (itr == pThis) continue;
			if (itr->GetTouchBase() == GotBase::Second) isTight = true;
		}
		break;
	case GotBase::Second:
		for (auto itr : m_pAllRunners)
		{
			if (itr == pThis) continue;
			if (itr->GetTouchBase() == GotBase::Third) isTight = true;
		}
		break;
	case GotBase::Third:
		if (GetScene()->GetGameObject<CBall>()->GetIsFryBall()) isTight = true;
		break;
	default:
		break;
	}

	if (isTight) TightRunnerNext(pThis);

	return isTight;
}

CRunner* CRunning::GetRunnerFromOldBase(GotBase base)
{
	for (auto itr : m_pAllRunners)
	{
		if (itr->GetOldBase() == base)
			return itr;
	}

	return nullptr;
}

void CRunning::TightRunnerNext(CRunner* pThis)
{
	CScene* pScene = GetScene();
	CBall* pBall = pScene->GetGameObject<CBall>();
	if (!pBall->GetIsFryChatch() && pBall->GetCaught())
	{
		if (m_pAllRunners.size() >= 2)
		{
			auto findItr = std::find(m_pAllRunners.rbegin(), m_pAllRunners.rend(), pThis);
			if (findItr == m_pAllRunners.rend()) return;
			std::advance(findItr, 1);
			if (findItr == m_pAllRunners.rend()) return;
			(*findItr)->SetStatus(RunnerStatus::CompulsoryToNext);
		}
	}
}

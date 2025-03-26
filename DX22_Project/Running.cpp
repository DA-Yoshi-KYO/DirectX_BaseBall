#include "Running.h"
#include "BallCount.h"
#include "Field.h"
#include "Ball.h"

CRunning::CRunning()
{
	m_pModel = std::make_unique<Model>();

}

CRunning::~CRunning()
{
}

void CRunning::Update()
{
	CBallCount* pBallCount = CBallCount::GetInstance().get();
	CField* pField = CField::GetInstance().get();
	CBall* pBall = CBall::GetInstance().get();
	
	switch (pBall->GetPhase())
	{
	case BallPhase::Batting:
		for (int i = 0; i < (int)RunnerKind::Max - 1; i++)
		{
			m_tRunnerParam[i].m_bAlive = pBallCount->GetBaseState(i);
			if (m_tRunnerParam[i].m_bAlive)
			{
				m_tRunnerParam[i].m_tModelParam.pos = pField->GetBasePos((BaseKind)i);
			}
		}
		m_tRunnerParam[(int)RunnerKind::BatterRunner].m_tModelParam.pos = pField->GetBasePos(BaseKind::Home);
		m_tRunnerParam[(int)RunnerKind::BatterRunner].m_bAlive = true;
		break;
	case  BallPhase::InPlay:
		for (int i = 0; i < (int)RunnerKind::Max; i++)
		{
			switch (i)
			{
			case (int)RunnerKind::FirstRunner:
				break;
			case (int)RunnerKind::SecondRunner:
				break;
			case (int)RunnerKind::ThirdRunner:
				break;
			case (int)RunnerKind::BatterRunner:
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}

}

void CRunning::Draw()
{
	CBallCount* pBallCount = CBallCount::GetInstance().get();
	CField* pField = CField::GetInstance().get();
	CBall* pBall = CBall::GetInstance().get();

	for (int i = 0; i < (int)RunnerKind::Max; i++)
	{
		if (pBallCount->GetBaseState(i))
		{
			if (pBall->GetPhase() == BallPhase::Batting && i == (int)RunnerKind::BatterRunner)continue;
			SetModel(m_tRunnerParam->m_tModelParam, m_pModel.get());
		}
	}
}

void CRunning::SetModel(ModelParam param, Model* model, bool isAnime)
{
}

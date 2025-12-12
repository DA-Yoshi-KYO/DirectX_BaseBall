#include "Runner.h"
#include "ModelRenderer.h"
#include "Main.h"
#include "Oparation.h"
#include "Ball.h"
#include "GameManager.h"
#include "Input.h"

CRunner::CRunner()
	: CGameObject()
	, m_eNowBase(GotBase::None), m_eTempBase(GotBase::None)
	, m_eCurrentRunnerKind(RunnerKind::BatterToFirst)
	, m_pCollision(nullptr),m_bIsStop(false), m_bFrontMove(true)
	, m_f3TargetPos(), m_bRunOut(true), m_bBackTempBase(false)
	, m_fStiffTime(0.0f), m_fSpeed(1.0f), m_bBatterRunner(true)
	, m_bAutoStart(false)
{
	m_tParam.m_f3Pos = DirectX::XMFLOAT3(0.0f, -4.5f, -218.0f);
	m_tParam.m_f3Size = DirectX::XMFLOAT3(15.0f, 15.0f, 15.0f);
	m_tParam.m_f3Rotate.x = DirectX::XMConvertToRadians(90.0f);
}

CRunner::~CRunner()
{

}

void CRunner::Init()
{
	CModelRenderer* pRenderer = AddComponent<CModelRenderer>();
	pRenderer->Load(PATH_MODEL("Character.fbx"),0.2f);
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Object.cso"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_TexColor.cso"));
	pRenderer->LoadTexture(PATH_MODEL("ball.png"));

	m_pCollision = AddComponent<CCollisionBox>();
	m_pCollision->SetTag("Runner");
	m_pCollision->SetInfo(m_tParam.m_f3Pos, m_tParam.m_f3Size);

	auto BaseList = GetScene()->GetSameGameObject<CBase>();
	int i = 0;
	for (auto itr : BaseList)
	{
		m_f3TargetPos[int(itr->GetKind())] = itr->GetPos();
	}
	int nNo = CGameManager::GetInstance()->GetAttackDirecter()->GetPlayerNo();
	if (nNo == 1) m_tParam.m_f4Color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	else if (nNo == 2) m_tParam.m_f4Color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
}

void CRunner::Update()
{
	if (CGameManager::GetInstance()->GetPhase() != GamePhase::InPlay)return;

	if (!m_bBackTempBase)
	{
		UpdateInput();
		CheckRunnerAutoStart();
	}
	else UpdateBackTempBase();

	DirectX::XMFLOAT3 target = {};
	bool progressCheck = true;
	switch (m_eCurrentRunnerKind)
	{
	case RunnerKind::BatterToFirst:
		target = m_f3TargetPos[int(BaseKind::First)];
		break;
	case RunnerKind::StayFirst:
		m_eTempBase = GotBase::First;
		target = m_f3TargetPos[int(BaseKind::First)];
		progressCheck = false;
		break;
	case RunnerKind::FirstToSecond:
		if (m_bFrontMove) target = m_f3TargetPos[int(BaseKind::Second)];
		else target = m_f3TargetPos[int(BaseKind::First)];
		break;
	case RunnerKind::StaySecond:
		target = m_f3TargetPos[int(BaseKind::Second)];
		progressCheck = false;
		break;
	case RunnerKind::SecondToThird:
		if (m_bFrontMove) target = m_f3TargetPos[int(BaseKind::Third)];
		else target = m_f3TargetPos[int(BaseKind::Second)];
		break;
	case RunnerKind::StayThird:
		target = m_f3TargetPos[int(BaseKind::Third)];
		progressCheck = false;
		break;
	case RunnerKind::ThirdToHome:
		if (m_bFrontMove) target = m_f3TargetPos[int(BaseKind::Home)];
		else target = m_f3TargetPos[int(BaseKind::Third)];
		break;
	default:
		break;
	}

	DirectX::XMFLOAT2 f2TargetDistance = DirectX::XMFLOAT2(target.x - m_tParam.m_f3Pos.x, target.z - m_tParam.m_f3Pos.z);
	DirectX::XMVECTOR vecDistance = DirectX::XMLoadFloat2(&f2TargetDistance);
	DirectX::XMVECTOR vecLength = DirectX::XMVector2Length(vecDistance);
	float fLength = 0.0f;
	DirectX::XMStoreFloat(&fLength, vecLength);
	if (progressCheck)
	{
		if (fLength < 15.0f)
		{
			m_bIsStop = !CheckCanProgress();
		}
	}
	if (!m_bIsStop)
	{
		if (fLength > 0.1f)
		{
			DirectX::XMVECTOR vecDir = DirectX::XMVector2Normalize(vecDistance);
			DirectX::XMVECTOR vecVelocity = DirectX::XMVectorScale(vecDir, m_fSpeed);
			DirectX::XMFLOAT2 f2Velocity;
			DirectX::XMStoreFloat2(&f2Velocity, vecVelocity);
			m_tParam.m_f3Pos.x += f2Velocity.x;
			m_tParam.m_f3Pos.z += f2Velocity.y;
		}
	}

	if (GetScene()->GetGameObject<CBall>()->GetIsFryChatch())
	{
		if (!m_bRunOut) m_eNowBase = m_eTempBase;
		else
		{
			if (m_eTempBase != m_eNowBase) m_bBackTempBase = true;
			else m_bRunOut = false;
		}
	}
	else
	{
		m_eNowBase = m_eTempBase;
	}
	if (m_fStiffTime != 0.0f)
	{
		m_fStiffTime -= 1.0f / fFPS;
		m_fStiffTime = std::max(0.0f, m_fStiffTime);
	}

	// “–‚½‚è”»’èî•ñ‚ÌXV
	Collision::Box tBox;
	tBox.center = m_tParam.m_f3Pos;
	tBox.center.y += 2.0f;
	tBox.size = m_tParam.m_f3Size * 0.5f;
	tBox.size.y += 2.0f;
	m_pCollision->SetInfo(tBox);
}

void CRunner::OnCollision(CCollisionBase* other, std::string thisTag, Collision::Result result)
{
	std::string tag = other->GetTag();
	if (tag == "HomeBase" && m_eNowBase == GotBase::Third)
	{
		Destroy();
		CGameManager::GetInstance()->GetCountDirecter()->AddScore();
		return;
	}
	if (tag == "FirstBase")
	{
		m_eTempBase = GotBase::First;
		if (m_fStiffTime > 0.0f) return;
		if (!m_bAutoStart) m_eCurrentRunnerKind = RunnerKind::StayFirst;
		return;
	}
	if (tag == "SecondBase")
	{
		m_eTempBase = GotBase::Second;
		if (m_fStiffTime > 0.0f) return;
		if (!m_bAutoStart)m_eCurrentRunnerKind = RunnerKind::StaySecond;
		return;
	}
	if (tag == "ThirdBase")
	{
		m_eTempBase = GotBase::Third;
		if (m_fStiffTime > 0.0f) return;
		if (!m_bAutoStart)m_eCurrentRunnerKind = RunnerKind::StayThird;
		return;
	}
}

void CRunner::ResetPos()
{
	switch (m_eNowBase)
	{
	case GotBase::First:
		m_tParam.m_f3Pos = m_f3TargetPos[int(BaseKind::First)];
		break;
	case GotBase::Second:
		m_tParam.m_f3Pos = m_f3TargetPos[int(BaseKind::Second)];
		break;
	case GotBase::Third:
		m_tParam.m_f3Pos = m_f3TargetPos[int(BaseKind::Third)];
		break;
	default:
		break;
	}
	m_tParam.m_f3Pos.y = -4.0f;
}

void CRunner::SetRunnerSpeed(Quality speed)
{
	m_fSpeed = 0.1f + int(speed) * 0.1f;
}

void CRunner::CheckRunOut()
{
	switch (m_eCurrentRunnerKind)
	{
	case RunnerKind::StayFirst:
	case RunnerKind::StaySecond:
	case RunnerKind::StayThird:
		if (m_eNowBase == m_eTempBase) m_bRunOut = false;
		break;
	default:
		break;
	}
}

void CRunner::CheckRunnerAutoStart()
{
	if (m_bBatterRunner) return;
	CScene* pScene = GetScene();
	if (pScene->GetGameObject<CBall>()->GetIsFryBall()) return;
	std::vector<RunnerKind> eOtherRunnerKind;

	auto RunnerList = GetScene()->GetSameGameObject<CRunner>();
	for (auto itr : RunnerList)
	{
		if (itr != this) eOtherRunnerKind.push_back(itr->GetCurrentKind());
	}

	switch (m_eCurrentRunnerKind)
	{
	case RunnerKind::StayFirst:
		for (auto itr : eOtherRunnerKind)
		{
			if (itr == RunnerKind::BatterToFirst)
			{
				m_eCurrentRunnerKind = RunnerKind::FirstToSecond;
				m_bIsStop = false;
				m_bFrontMove = true;
				m_bAutoStart = true;
			}
		}
		break;
	case RunnerKind::StaySecond:
		for (auto itr : eOtherRunnerKind)
		{
			if (itr == RunnerKind::FirstToSecond)
			{
				m_eCurrentRunnerKind = RunnerKind::SecondToThird;
				m_bIsStop = false;
				m_bFrontMove = true;
				m_bAutoStart = true;
			}
		}
		break;
	case RunnerKind::StayThird:
		for (auto itr : eOtherRunnerKind)
		{
			if (itr == RunnerKind::SecondToThird)
			{
				m_eCurrentRunnerKind = RunnerKind::ThirdToHome;
				m_bIsStop = false;
				m_bFrontMove = true;
				m_bAutoStart = true;
			}
		}
		break;
	default:
		m_bAutoStart = false;
		break;
	}
}

void CRunner::GoToNextBase()
{
	switch (m_eNowBase)
	{
	case GotBase::None:
		m_eNowBase = GotBase::First;
		break;
	case GotBase::First:
		m_eNowBase = GotBase::Second;
		break;
	case GotBase::Second:
		m_eNowBase = GotBase::Third;
		break;
	case GotBase::Third:
		CGameManager::GetInstance()->GetCountDirecter()->AddScore();
		Destroy();
		break;
	case GotBase::Max:
		break;
	default:
		break;
	}
}

void CRunner::UpdateInput()
{
	if (m_fStiffTime > 0.0f) return;

	CAttackDirecter* pAttackDirecter = CGameManager::GetInstance()->GetAttackDirecter();
	int nAttackPlayerNo = pAttackDirecter->GetPlayerNo();
	if (IsKeyTrigger(nAttackPlayerNo, Input::Y))
	{
		m_fStiffTime = 1.0f;
		if (IsKeyPress(nAttackPlayerNo, Input::B))
		{
			m_bIsStop = true;
			return;
		}

		m_bIsStop = false;
		m_bFrontMove = true;
		switch (m_eCurrentRunnerKind)
		{
		case RunnerKind::StayFirst:
			m_eCurrentRunnerKind = RunnerKind::FirstToSecond;
			break;
		case RunnerKind::StaySecond:
			m_eCurrentRunnerKind = RunnerKind::SecondToThird;
			break;
		case RunnerKind::StayThird:
			m_eCurrentRunnerKind = RunnerKind::ThirdToHome;
			break;
		default:
			break;
		}
		return;
	}

	if (IsKeyTrigger(nAttackPlayerNo, Input::B))
	{
		m_fStiffTime = 1.0f;
		switch (m_eCurrentRunnerKind)
		{
		case RunnerKind::StayFirst:
		case RunnerKind::StaySecond:
		case RunnerKind::StayThird:
			return;
			break;
		default:
			break;
		}

		if (IsKeyPress(nAttackPlayerNo, Input::Y))
		{
			m_bIsStop = true;
			return;
		}

		m_bIsStop = false;
		m_bFrontMove = false;
		return;
	}
}

void CRunner::UpdateBackTempBase()
{
	if (m_eNowBase == m_eTempBase)
	{
		m_bBackTempBase = false;
		return;
	}

	m_bFrontMove = false;
	switch (m_eCurrentRunnerKind)
	{
	case RunnerKind::StaySecond:
		m_eCurrentRunnerKind = RunnerKind::FirstToSecond;
		break;
	case RunnerKind::StayThird:
		m_eCurrentRunnerKind = RunnerKind::SecondToThird;
		break;
	default:
		break;
	}
}

bool CRunner::CheckCanProgress()
{
	CScene* pScene = GetScene();
	auto RunnerList = pScene->GetSameGameObject<CRunner>();
	std::vector<RunnerKind> eRunnerKindList;
	eRunnerKindList.clear();
	for (auto itr : RunnerList)
	{
		if (itr != this) eRunnerKindList.push_back(itr->GetCurrentKind());
	}

	for (auto itr : eRunnerKindList)
	{
		switch (m_eCurrentRunnerKind)
		{
		case RunnerKind::BatterToFirst:
			if (itr == RunnerKind::StayFirst) return false;
			break;
		case RunnerKind::FirstToSecond:
			m_eCurrentRunnerKind = RunnerKind::FirstToSecond;
			if (itr == RunnerKind::StaySecond) return false;
			break;
		case RunnerKind::SecondToThird:
			m_eCurrentRunnerKind = RunnerKind::SecondToThird;
			if (itr == RunnerKind::StayThird) return false;
			break;
		case RunnerKind::ThirdToHome:
			m_eCurrentRunnerKind = RunnerKind::ThirdToHome;
			if (pScene->GetGameObject<CBall>()->GetIsFryBall()) return false;
			break;
		default:
			return true;
			break;
		}
	}

	return true;
}

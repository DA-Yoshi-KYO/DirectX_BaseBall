#include "Runner.h"
#include "ModelRenderer.h"
#include "Main.h"
#include "Oparation.h"
#include "Ball.h"
#include "GameManager.h"
#include "Input.h"
#include "Running.h"

CRunner::CRunner()
	: CGameObject()
	, m_eCurrentBase(GotBase::None), m_eOldBase(GotBase::None), m_eTouchBase(GotBase::None)
	, m_eStatus(RunnerStatus::Stop)
	, m_pCollision(nullptr), m_bIsStop(false), m_bFrontMove(true)
	, m_f3TargetPos(), m_bRunOut(true), m_bBackTempBase(false)
	, m_fStiffTime(0.0f), m_fSpeed(1.0f), m_bBatterRunner(true)
	, m_bAutoStart(false), m_bTightRunner(false), m_pRunning(nullptr)
{
	m_tParam.m_f3Pos = DirectX::XMFLOAT3(0.0f, -4.5f, -218.0f);
	m_tParam.m_f3Size = DirectX::XMFLOAT3(15.0f, 15.0f, 15.0f);
	m_tParam.m_f3OffSet = { 0.0f,-3.0f,0.0f };
	m_tParam.m_f3Scale = { 2.0f,2.0f,2.0f };
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
	//if (nNo == 1) m_tParam.m_f4Color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	//else if (nNo == 2) m_tParam.m_f4Color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	m_pRunning = CGameManager::GetInstance()->GetAttackDirecter()->GetRunning();
}

void CRunner::Update()
{
	if (CGameManager::GetInstance()->GetPhase() != GamePhase::InPlay) return;

	switch (m_eStatus)
	{
	case RunnerStatus::Stop:
		break;
	case RunnerStatus::ToNext:
		UpdateToNext();
		break;
	case RunnerStatus::ToBack:
		UpdateToBack();
		break;
	case RunnerStatus::BackAll:
		break;
	case RunnerStatus::CompulsoryToNext:
		UpdateCompulsoryToNext();
		break;
	case RunnerStatus::Max:
		break;
	default:
		break;
	}

	// “–‚½‚è”»’èî•ñ‚ÌXV
	Collision::Box tBox;
	tBox.center = m_tParam.m_f3Pos;
	tBox.center.y += 2.0f;
	tBox.size = m_tParam.m_f3Size * 0.5f;
	tBox.size.y += 2.0f;
	m_pCollision->SetInfo(tBox);
}

void CRunner::OnDestroy()
{
	m_pRunning->RemoveRunner(this);
}

void CRunner::OnCollision(CCollisionBase* other, std::string thisTag, Collision::Result result)
{
	std::string tag = other->GetTag();
	if (tag == "HomeBase" && m_eCurrentBase == GotBase::Third)
	{
		Destroy();
		CGameManager::GetInstance()->GetCountDirecter()->AddScore();
		return;
	}
	if (tag == "FirstBase")
	{
		m_eTouchBase = GotBase::First;
		return;
	}
	if (tag == "SecondBase")
	{
		m_eTouchBase = GotBase::Second;
		return;
	}
	if (tag == "ThirdBase")
	{
		m_eTouchBase = GotBase::Third;
		return;
	}
}

void CRunner::OnCollisionExit(CCollisionBase* other, std::string thisTag)
{
	std::string tag = other->GetTag();

	if (tag == "FirstBase" ||
		tag == "SecondBase" ||
		tag == "ThirdBase")
	{
		m_eTouchBase = GotBase::None;
		return;
	}
}

void CRunner::ResetPos()
{
	switch (m_eCurrentBase)
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

	m_eOldBase = m_eCurrentBase;
}

void CRunner::SetRunnerSpeed(Quality speed)
{
	m_fSpeed = 0.1f + int(speed) * 0.1f;
}

void CRunner::CheckRunOut()
{

}

void CRunner::UpdateToNext()
{
	if (int(m_eTouchBase) == int(m_eCurrentBase) + 1)
	{
		m_eStatus = RunnerStatus::Stop;
		m_eCurrentBase = m_eTouchBase;
		return;
	}
	DirectX::XMFLOAT2 f2Distance{};
	switch (m_eCurrentBase)
	{
	case GotBase::None:
		f2Distance.x = m_f3TargetPos[int(BaseKind::First)].x - m_tParam.m_f3Pos.x;
		f2Distance.y = m_f3TargetPos[int(BaseKind::First)].z - m_tParam.m_f3Pos.z;
		break;
	case GotBase::First:
		f2Distance.x = m_f3TargetPos[int(BaseKind::Second)].x - m_tParam.m_f3Pos.x;
		f2Distance.y = m_f3TargetPos[int(BaseKind::Second)].z - m_tParam.m_f3Pos.z;
		break;
	case GotBase::Second:
		f2Distance.x = m_f3TargetPos[int(BaseKind::Third)].x - m_tParam.m_f3Pos.x;
		f2Distance.y = m_f3TargetPos[int(BaseKind::Third)].z - m_tParam.m_f3Pos.z;
		break;
	case GotBase::Third:
		f2Distance.x = m_f3TargetPos[int(BaseKind::Home)].x - m_tParam.m_f3Pos.x;
		f2Distance.y = m_f3TargetPos[int(BaseKind::Home)].z - m_tParam.m_f3Pos.z;
		break;
	default:
		break;
	}	
	
	DirectX::XMVECTOR vecDist = DirectX::XMLoadFloat2(&f2Distance);
	DirectX::XMVECTOR vecLength = DirectX::XMVector2Length(vecDist);
	float fLength = 0.0f;
	DirectX::XMStoreFloat(&fLength, vecLength);
	if (fLength < 20.0f)
	{
		if (m_pRunning->IsTight(this))
		{
			return;
		}
	}

	DirectX::XMVECTOR vecDir = DirectX::XMVector2Normalize(vecDist);
	DirectX::XMVECTOR vecVel = DirectX::XMVectorScale(vecDir, m_fSpeed);
	DirectX::XMFLOAT2 f2Vel{};
	DirectX::XMStoreFloat2(&f2Vel, vecVel);
	m_tParam.m_f3Pos.x += f2Vel.x;
	m_tParam.m_f3Pos.z += f2Vel.y;
}

void CRunner::UpdateToBack()
{
	DirectX::XMFLOAT2 f2Distance{};
	if (int(m_eTouchBase) == int(m_eCurrentBase))
	{
		m_eStatus = RunnerStatus::Stop;
		return;
	}
	switch (m_eCurrentBase)
	{
	case GotBase::None:
		f2Distance.x = m_f3TargetPos[int(BaseKind::First)].x - m_tParam.m_f3Pos.x;
		f2Distance.y = m_f3TargetPos[int(BaseKind::First)].z - m_tParam.m_f3Pos.z;
		break;
	case GotBase::First:
		f2Distance.x = m_f3TargetPos[int(BaseKind::Second)].x - m_tParam.m_f3Pos.x;
		f2Distance.y = m_f3TargetPos[int(BaseKind::Second)].z - m_tParam.m_f3Pos.z;
		break;
	case GotBase::Second:
		f2Distance.x = m_f3TargetPos[int(BaseKind::Third)].x - m_tParam.m_f3Pos.x;
		f2Distance.y = m_f3TargetPos[int(BaseKind::Third)].z - m_tParam.m_f3Pos.z;
		break;
	case GotBase::Third:
		f2Distance.x = m_f3TargetPos[int(BaseKind::Home)].x - m_tParam.m_f3Pos.x;
		f2Distance.y = m_f3TargetPos[int(BaseKind::Home)].z - m_tParam.m_f3Pos.z;
		break;
	default:
		break;
	}

	DirectX::XMVECTOR vecDir = DirectX::XMVector2Normalize(DirectX::XMLoadFloat2(&f2Distance));
	DirectX::XMVECTOR vecVel = DirectX::XMVectorScale(vecDir, m_fSpeed);
	DirectX::XMFLOAT2 f2Vel{};
	DirectX::XMStoreFloat2(&f2Vel, vecVel);
	m_tParam.m_f3Pos.x += f2Vel.x;
	m_tParam.m_f3Pos.z += f2Vel.y;
}

void CRunner::UpdateCompulsoryToNext()
{
	UpdateToNext();
}

void CRunner::CheckRunnerAutoStart()
{
	//if (m_bBatterRunner) return;
	//CScene* pScene = GetScene();
	//if (pScene->GetGameObject<CBall>()->GetIsFryBall()) return;
	//std::vector<RunnerKind> eOtherRunnerKind;

	//auto RunnerList = GetScene()->GetSameGameObject<CRunner>();
	//for (auto itr : RunnerList)
	//{
	//	if (itr != this) eOtherRunnerKind.push_back(itr->GetCurrentKind());
	//}

	//switch (m_eCurrentRunnerKind)
	//{
	//case RunnerKind::StayFirst:
	//	for (auto itr : eOtherRunnerKind)
	//	{
	//		if (itr == RunnerKind::BatterToFirst)
	//		{
	//			m_eCurrentRunnerKind = RunnerKind::FirstToSecond;
	//			m_bIsStop = false;
	//			m_bFrontMove = true;
	//			m_bAutoStart = true;
	//		}
	//	}
	//	break;
	//case RunnerKind::StaySecond:
	//	for (auto itr : eOtherRunnerKind)
	//	{
	//		if (itr == RunnerKind::FirstToSecond)
	//		{
	//			m_eCurrentRunnerKind = RunnerKind::SecondToThird;
	//			m_bIsStop = false;
	//			m_bFrontMove = true;
	//			m_bAutoStart = true;
	//		}
	//	}
	//	break;
	//case RunnerKind::StayThird:
	//	for (auto itr : eOtherRunnerKind)
	//	{
	//		if (itr == RunnerKind::SecondToThird)
	//		{
	//			m_eCurrentRunnerKind = RunnerKind::ThirdToHome;
	//			m_bIsStop = false;
	//			m_bFrontMove = true;
	//			m_bAutoStart = true;
	//		}
	//	}
	//	break;
	//default:
	//	m_bAutoStart = false;
	//	break;
	//}
}

void CRunner::GoToNextBase()
{
	switch (m_eCurrentBase)
	{
	case GotBase::None:
		m_eCurrentBase = GotBase::First;
		break;
	case GotBase::First:
		m_eCurrentBase = GotBase::Second;
		break;
	case GotBase::Second:
		m_eCurrentBase = GotBase::Third;
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
	/*if (m_fStiffTime > 0.0f) return;

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
	}*/
}

void CRunner::UpdateBackTempBase()
{
	/*if (m_eNowBase == m_eTempBase)
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
	}*/
}

bool CRunner::CheckCanProgress()
{
	return true;	
	/*CScene* pScene = GetScene();
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
		case RunnerKind::FirstToSecond:wwwww
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

	return true;*/
}

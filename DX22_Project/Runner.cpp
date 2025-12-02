#include "Runner.h"
#include "ModelRenderer.h"
#include "Main.h"
#include "Oparation.h"
#include "Ball.h"

CRunner::CRunner()
	: CGameObject()
	, m_eNowBase(GotBase::None), m_eTempBase(GotBase::None)
	, m_eCurrentRunnerKind(RunnerKind::BatterToFirst)
	, m_pCollision(nullptr),m_bIsStop(false), m_bFrontMove(true)
	, m_f3TargetPos(), m_bRunOut(true), m_bBackTempBase(false)
{
	m_tParam.m_f3Pos = DirectX::XMFLOAT3(0.0f, -4.5f, -218.0f);
	m_tParam.m_f3Size = DirectX::XMFLOAT3(5.0f, 5.0f, 5.0f);
}

CRunner::~CRunner()
{

}

void CRunner::Init()
{
	CModelRenderer* pRenderer = AddComponent<CModelRenderer>();
	pRenderer->Load(PATH_MODEL("Ball.obj"));
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
}

void CRunner::Update()
{
	if (!m_bBackTempBase) UpdateInput();
	else UpdateBackTempBase();

	DirectX::XMFLOAT3 target = {};
	switch (m_eCurrentRunnerKind)
	{
	case RunnerKind::BatterToFirst:
		target = m_f3TargetPos[int(BaseKind::First)];
		break;
	case RunnerKind::StayFirst:
		m_eTempBase = GotBase::First;
		m_bIsStop = true;
		break;
	case RunnerKind::FirstToSecond:
		if (m_bFrontMove) target = m_f3TargetPos[int(BaseKind::Second)];
		else target = m_f3TargetPos[int(BaseKind::First)];
		break;
	case RunnerKind::StaySecond:
		m_bIsStop = true;
		break;
	case RunnerKind::SecondToThird:
		if (m_bFrontMove) target = m_f3TargetPos[int(BaseKind::Third)];
		else target = m_f3TargetPos[int(BaseKind::Second)];
		break;
	case RunnerKind::StayThird:
		m_bIsStop = true;
		break;
	case RunnerKind::ThirdToHome:
		if (m_bFrontMove) target = m_f3TargetPos[int(BaseKind::Home)];
		else target = m_f3TargetPos[int(BaseKind::Third)];
		break;
	default:
		break;
	}

	if (!m_bIsStop)
	{
		DirectX::XMFLOAT2 f2TargetDistance = DirectX::XMFLOAT2(target.x - m_tParam.m_f3Pos.x, target.z - m_tParam.m_f3Pos.z);
		DirectX::XMVECTOR vecDir = DirectX::XMVector2Normalize(DirectX::XMLoadFloat2(&f2TargetDistance));
		DirectX::XMVECTOR vecVelocity = DirectX::XMVectorScale(vecDir, 0.1f);
		DirectX::XMFLOAT2 f2Velocity;
		DirectX::XMStoreFloat2(&f2Velocity, vecVelocity);
		m_tParam.m_f3Pos.x += f2Velocity.x;
		m_tParam.m_f3Pos.z += f2Velocity.y;
	}
	else
	{

	}

	if (!GetScene()->GetGameObject<CBall>()->GetIsFryBall())
	{
		if (!m_bRunOut) m_eNowBase = m_eTempBase;
		else
		{
			if (m_eTempBase != m_eNowBase) m_bBackTempBase = true;
			else m_bRunOut = false;
		}
	}
	m_pCollision->SetInfo(m_tParam.m_f3Pos, m_tParam.m_f3Size);
}

void CRunner::OnCollision(CCollisionBase* other, std::string thisTag, Collision::Result result)
{
	std::string tag = other->GetTag();
	if (tag == "HomeBase" && m_eNowBase == GotBase::Third)
	{
		// TODO:ìæì_èàóùÇí«â¡
		Destroy();
		return;
	}
	if (tag == "FirstBase")
	{
		m_eTempBase = GotBase::First;
		m_eCurrentRunnerKind = RunnerKind::StayFirst;
	}
	if (tag == "SecondBase")
	{
		m_eTempBase = GotBase::Second;
		m_eCurrentRunnerKind = RunnerKind::StaySecond;
	}
	if (tag == "ThirdBase")
	{
		m_eTempBase = GotBase::Third;
		m_eCurrentRunnerKind = RunnerKind::StayThird;
	}
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

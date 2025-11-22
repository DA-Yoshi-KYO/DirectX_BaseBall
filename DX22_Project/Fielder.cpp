#include "Fielder.h"
#include "ModelRenderer.h"
#include "Main.h"
#include "Field.h"
#include "Ball.h"
#include "GameManager.h"
#include "Input.h"

constexpr float ce_fDifencePower = 0.2f;	// 守備移動速度
constexpr float ce_fDifence = 0.4f;			// 守備操作速度
constexpr float ce_fThrowingPower = 2.0f;	// 送球の強さ

CFielder::CFielder()
	: m_pFielderData(nullptr), m_ePosition(Positions::Pitcher)
	, m_bIsOparation(false), m_bChatch(false)
	, m_bMostNearToBase{}, m_pCollision{}
{
	m_tParam.m_f3Size = { 5.0f,5.0f,5.0f };
}

CFielder::~CFielder()
{

}

void CFielder::Init()
{
	CModelRenderer* pRenderer = AddComponent<CModelRenderer>();
	pRenderer->Load(PATH_MODEL("Ball.obj"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Object.cso"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_TexColor.cso"));

	m_pCollision = AddComponent<CCollisionBox>();
	m_pCollision->SetTag("Fielder");
	m_pCollision->SetInfo(m_tParam.m_f3Pos, m_tParam.m_f3Size);

}

void CFielder::Update()
{
	CGameManager* pGameManager = CGameManager::GetInstance();
	GamePhase ePhase = pGameManager->GetPhase();
	if (ePhase != GamePhase::InPlay) return;
	int DefencePlayer = pGameManager->GetDefenceManager()->GetPlayerNo();

	if (m_bIsOparation)
	{
		CBall* pBall = GetScene()->GetGameObject<CBall>();
		// 移動処理	
		float fMovePow = int(m_pFielderData->GetFielderData().m_eDefence) * (0.45f / 7.0f) + 0.2f;
		if (IsKeyPress(DefencePlayer, Input::Up))	m_tParam.m_f3Pos.z -= fMovePow;
		if (IsKeyPress(DefencePlayer, Input::Down)) m_tParam.m_f3Pos.z += fMovePow;
		if (IsKeyPress(DefencePlayer, Input::Left)) m_tParam.m_f3Pos.x -= fMovePow;
		if (IsKeyPress(DefencePlayer, Input::Right))m_tParam.m_f3Pos.x += fMovePow;

		if (m_bChatch)
		{
			pBall->SetPos(m_tParam.m_f3Pos);
			// 送球処理
			if (IsKeyPress(DefencePlayer, Input::B)) Throwing(BaseKind::First);
			if (IsKeyPress(DefencePlayer, Input::Y)) Throwing(BaseKind::Second);
			if (IsKeyPress(DefencePlayer, Input::X)) Throwing(BaseKind::Third);
			if (IsKeyPress(DefencePlayer, Input::A)) Throwing(BaseKind::Home);
		}
	}
	else
	{
		BaseCover();
	}

	// 当たり判定情報の更新
	m_pCollision->SetInfo(m_tParam.m_f3Pos, m_tParam.m_f3Size);

	CGameObject::Update();
}

void CFielder::ResetPos()
{
	// 計算に使う変数の定義
	CField* pField = GetScene()->GetGameObject<CField>();
	DirectX::XMFLOAT3 fFieldPos = pField->GetPos();
	DirectX::XMFLOAT3 fFieldSizeMin = pField->GetSize();
	DirectX::XMFLOAT3 fFieldSize = { fFieldSizeMin.x * 8.0f , fFieldSizeMin.y , fFieldSizeMin.z * 8.0f };
	DirectX::XMFLOAT3 fFieldPosLine = { fFieldSize.x / 10.0f,fFieldSize.y ,fFieldSize.z / 10.0f };

	switch (m_ePosition)
	{
	case Positions::Pitcher:
		m_tParam.m_f3Pos = { fFieldPos.x,fFieldPos.y, fFieldPos.z + fFieldPosLine.z * 2.5f };
		break;
	case Positions::Chatcher:
		m_tParam.m_f3Pos = { fFieldPos.x,fFieldPos.y, fFieldPos.z + fFieldPosLine.z * 7.3f };
		break;
	case Positions::First:
		m_tParam.m_f3Pos = { fFieldPos.x - fFieldPosLine.x * 1.7f,fFieldPos.y, fFieldPos.z + fFieldPosLine.z * 2.5f };
		break;
	case Positions::Second:
		m_tParam.m_f3Pos = { fFieldPos.x - fFieldPosLine.x * 0.9f,fFieldPos.y, fFieldPos.z + fFieldPosLine.z * 1.2f };
		break;
	case Positions::Third:
		m_tParam.m_f3Pos = { fFieldPos.x + fFieldPosLine.x * 1.7f,fFieldPos.y, fFieldPos.z + fFieldPosLine.z * 2.5f };
		break;
	case Positions::Short:
		m_tParam.m_f3Pos = { fFieldPos.x + fFieldPosLine.x * 0.9f,fFieldPos.y, fFieldPos.z + fFieldPosLine.z * 1.2f };
		break;
	case Positions::Left:
		m_tParam.m_f3Pos = { fFieldPos.x + fFieldPosLine.x * 2.3f,fFieldPos.y, fFieldPos.z - fFieldPosLine.z * 1.7f };
		break;
	case Positions::Center:
		m_tParam.m_f3Pos = { fFieldPos.x,fFieldPos.y, fFieldPos.z - fFieldPosLine.z * 2.3f };
		break;
	case Positions::Right:
		m_tParam.m_f3Pos = { fFieldPos.x - fFieldPosLine.x * 2.3f,fFieldPos.y, fFieldPos.z - fFieldPosLine.z * 1.7f };
		break;
	default:
		break;
	}
}

void CFielder::OnCollision(CCollisionBase* other, std::string thisTag, Collision::Result result)
{
	CBall* pBall = dynamic_cast<CBall*>(other->GetGameObject());
	if (!pBall) return;
	m_bChatch = true;
	m_bIsOparation = true;
	pBall->SetActive(false);
	bool isFry = pBall->GetIsFryBall();
	if (isFry)
	{
		CGameManager::GetInstance()->GetCountManager()->AddOutCount();
	}
}

void CFielder::SetData(CFielderData* fielder, Positions position)
{
	m_pFielderData = fielder;
	m_ePosition = position;
}

bool CFielder::SetBaseCoverFrag(int baseIndex, bool frag)
{
	if (frag)
	{
		if (m_bIsOparation) return false;
		for (int i = 0; i < (int)BaseKind::Max; i++)
		{
			if (i != baseIndex)
			{
				if (m_bMostNearToBase[i]) return false;
			}
		}
	}
	m_bMostNearToBase[baseIndex] = frag;

	return true;
}

void CFielder::BaseCover()
{
	std::list<CBase*> pField = GetScene()->GetSameGameObject<CBase>();
	int index = 0;
	for (auto itr : pField)
	{
		if (!m_bMostNearToBase[index]) continue;

		DirectX::XMFLOAT3 fBasePos = itr->GetPos();
		DirectX::XMFLOAT3 fBaseSize = itr->GetSize();
		Collision::Info2D member;
		Collision::Info2D base;
		member.type = Collision::eSquare;
		member.square.pos = { m_tParam.m_f3Pos.x,m_tParam.m_f3Pos.z };
		member.square.size = { m_tParam.m_f3Size.x,m_tParam.m_f3Size.z };
		base.type = Collision::eSquare;
		base.square.pos = { fBasePos.x,fBasePos.z };
		base.square.size = { fBaseSize.x,fBaseSize.z };
		if (Collision::Hit2D(member, base).isHit)
		{
			itr->SetBaseCover(true);
			continue;
		}
		else
		{
			itr->SetBaseCover(false);
		}
		DirectX::XMVECTOR vecBasePos = DirectX::XMLoadFloat3(&fBasePos);
		DirectX::XMVECTOR vecMemberPos = DirectX::XMLoadFloat3(&m_tParam.m_f3Pos);
		DirectX::XMVECTOR vecDirection = DirectX::XMVectorSubtract(vecBasePos, vecMemberPos);
		vecDirection = DirectX::XMVector3Normalize(vecDirection);
		vecDirection = DirectX::XMVectorScale(vecDirection, ce_fDifencePower);
		DirectX::XMFLOAT3 fMove;
		DirectX::XMStoreFloat3(&fMove, vecDirection);
		m_tParam.m_f3Pos.x += fMove.x;
		m_tParam.m_f3Pos.z += fMove.z;

		++index;
	}
}

void CFielder::Throwing(BaseKind kind)
{
	std::list<CBase*> pField = GetScene()->GetSameGameObject<CBase>();
	CBase* pBase = nullptr;
	for (auto itr : pField)
	{
		if (itr->GetKind() == kind) pBase = itr;
	}
	if (!pBase) return;

	CScene* pScene = GetScene();
	CBall* pBall = pScene->GetGameObject<CBall>();
	CGameManager* pManager = CGameManager::GetInstance();
	CTeamDirector* pTeamManager = pManager->GetTeamManager(pManager->GetDefenceManager()->GetPlayerNo());

	DirectX::XMFLOAT3 fBaseCoverPos = m_tParam.m_f3Pos;
	DirectX::XMFLOAT3 fBallPos = pBall->GetPos();
	DirectX::XMVECTOR vecBallPos = DirectX::XMLoadFloat3(&fBallPos);
	DirectX::XMVECTOR vecBaseCoverPos = DirectX::XMLoadFloat3(&fBaseCoverPos);
	DirectX::XMVECTOR vecDirection = DirectX::XMVectorSubtract(vecBaseCoverPos, vecBallPos);
	DirectX::XMVECTOR vecThrowLength = DirectX::XMVector3Length(vecDirection);
	float fThrowAngle = DirectX::XMVectorGetX(vecThrowLength);
	fThrowAngle /= 400.0f;

	vecDirection = DirectX::XMVector3Normalize(vecDirection);
	vecBallPos = DirectX::XMVectorAdd(vecBallPos, DirectX::XMVectorScale(vecDirection, 5.0f));
	DirectX::XMFLOAT3 fNewBallPos;
	DirectX::XMStoreFloat3(&fNewBallPos, vecBallPos);
	pBall->SetPos(fNewBallPos);
	float fThrowPow = int(pTeamManager->GetTeam()->GetPositionFielder(m_ePosition)->GetFielderData().m_eThrowing) * (3.5f / 7.0f) + 0.5f;
	vecDirection = DirectX::XMVectorScale(vecDirection, ce_fThrowingPower);
	DirectX::XMFLOAT3 fVelocity;
	DirectX::XMStoreFloat3(&fVelocity, vecDirection);

	fVelocity.y = fThrowAngle;
	pBall->SetVelocity(fVelocity);
	m_bChatch = false;
}

#include "Fielding.h"
#include "Camera.h"
#include "Main.h"
#include "Ball.h"
#include "BallCount.h"
#include "Input.h"
#include "Running.h"

constexpr  float ce_fDifencePower = 0.2f;
constexpr  float ce_fThrowingPower = 4.0f;

CFielding::CFielding()
	: m_bHold(false), m_nOperationNo(0), m_nBaseNearNo{ -1,-1,-1,-1 }
	, m_bBaseCovered{}
{
	for (int i = 0; i < (int)FieldMember::Max; i++)
	{
		m_pFieldMember[i] = std::make_unique<Model>();
		m_tParam[i].pos = { WORLD_AJUST,WORLD_AJUST, WORLD_AJUST };
		m_tParam[i].size = { 5.0f,5.0f,5.0f };
		m_tParam[i].rotate = { 0.0f,0.0f,0.0f };
		m_Collision[i].type = Collision::eBox;
		m_Collision[i].box.center = m_tParam[i].pos;
		m_Collision[i].box.size = m_tParam[i].size;
	}
	if (!m_pFieldMember[(int)FieldMember::Pitcher]->Load(MODELPASS("ball.obj"))) ERROR_MESSAGE("");
	if(!m_pFieldMember[(int)FieldMember::Chatcher]->Load(MODELPASS("ball.obj")))ERROR_MESSAGE("");
	if(!m_pFieldMember[(int)FieldMember::First]->Load(MODELPASS("ball.obj")))ERROR_MESSAGE("");
	if(!m_pFieldMember[(int)FieldMember::Second]->Load(MODELPASS("ball.obj")))ERROR_MESSAGE("");
	if(!m_pFieldMember[(int)FieldMember::Third]->Load(MODELPASS("ball.obj")))ERROR_MESSAGE("");
	if(!m_pFieldMember[(int)FieldMember::Short]->Load(MODELPASS("ball.obj")))ERROR_MESSAGE("");
	if(!m_pFieldMember[(int)FieldMember::Left]->Load(MODELPASS("ball.obj")))ERROR_MESSAGE("");
	if(!m_pFieldMember[(int)FieldMember::Center]->Load(MODELPASS("ball.obj")))ERROR_MESSAGE("");
	if(!m_pFieldMember[(int)FieldMember::Right]->Load(MODELPASS("ball.obj")))ERROR_MESSAGE("");
}

CFielding::~CFielding()
{
}

void CFielding::Update()
{
	CField* pField = CField::GetInstance().get();
	CBall* pBall = CBall::GetInstance().get();
	CBallCount* pBallCount = CBallCount::GetInstance().get();
	DirectX::XMFLOAT3 fFieldPos = pField->GetPos();
	fFieldPos.y = WORLD_AJUST + ce_fGroundY;
	DirectX::XMFLOAT3 fFieldSizeMin = pField->GetSize();
	DirectX::XMFLOAT3 fFieldSize = { fFieldSizeMin.x * 8.0f , fFieldSizeMin.y * 8.0f , fFieldSizeMin.z * 8.0f };

	DirectX::XMFLOAT3 fFieldPosLine = {fFieldSize.x / 10.0f,fFieldSize.y / 10.0f ,fFieldSize.z / 10.0f };
	static int nHoldNo = 0;

	switch (CBall::GetInstance()->GetPhase())
	{
	case BallPhase::Batting:
		m_tParam[(int)FieldMember::Pitcher].pos = { fFieldPos.x,fFieldPos.y, fFieldPos.z + fFieldPosLine.z * 4.5f };
		m_tParam[(int)FieldMember::Chatcher].pos = { fFieldPos.x,fFieldPos.y, fFieldPos.z + fFieldPosLine.z * 7.3f };
		m_tParam[(int)FieldMember::First].pos = { fFieldPos.x - fFieldPosLine.x * 1.7f,fFieldPos.y, fFieldPos.z + fFieldPosLine.z * 4.0f };
		m_tParam[(int)FieldMember::Second].pos = { fFieldPos.x - fFieldPosLine.x * 0.9f,fFieldPos.y, fFieldPos.z + fFieldPosLine.z * 2.8f };
		m_tParam[(int)FieldMember::Third].pos = { fFieldPos.x + fFieldPosLine.x * 1.7f,fFieldPos.y, fFieldPos.z + fFieldPosLine.z * 4.0f };
		m_tParam[(int)FieldMember::Short].pos = { fFieldPos.x + fFieldPosLine.x * 0.9f,fFieldPos.y, fFieldPos.z + fFieldPosLine.z * 2.5f };
		m_tParam[(int)FieldMember::Left].pos = { fFieldPos.x + fFieldPosLine.x * 2.3f,fFieldPos.y, fFieldPos.z - fFieldPosLine.z * 1.3f };
		m_tParam[(int)FieldMember::Center].pos = { fFieldPos.x,fFieldPos.y, fFieldPos.z - fFieldPosLine.z * 2.0f };
		m_tParam[(int)FieldMember::Right].pos = { fFieldPos.x - fFieldPosLine.x * 2.3f,fFieldPos.y, fFieldPos.z - fFieldPosLine.z * 1.3f };
		m_bHold = false;
		break;
	case BallPhase::InPlay:
		if (!pBallCount->GetEndInplay())
		{
			if (!m_bHold) m_nOperationNo = OperationSearch();
			else
			{
				m_nOperationNo = nHoldNo;
				pBall->SetPos(m_tParam[m_nOperationNo].pos);
				m_fThrowDirection = {};
			}

			if (IsKeyPress('W')) m_tParam[m_nOperationNo].pos.z -= 0.5f;
			if (IsKeyPress('S')) m_tParam[m_nOperationNo].pos.z += 0.5f;
			if (IsKeyPress('D')) m_tParam[m_nOperationNo].pos.x -= 0.5f;
			if (IsKeyPress('A')) m_tParam[m_nOperationNo].pos.x += 0.5f;
			for (int i = 0; i < (int)BaseKind::Max; i++)
			{
				m_nBaseNearNo[i] = -1;
			}
			m_nBaseNearNo[(int)BaseKind::First] = BaseSearch(BaseKind::First);
			m_nBaseNearNo[(int)BaseKind::Second] = BaseSearch(BaseKind::Second);
			m_nBaseNearNo[(int)BaseKind::Third] = BaseSearch(BaseKind::Third);
			m_nBaseNearNo[(int)BaseKind::Home] = BaseSearch(BaseKind::Home);
			BaseCover();


			if (!m_bHold)
			{
				Collision::Info ballCollision = pBall->GetCollision();
				for (int i = 0; i < (int)FieldMember::Max; i++)
				{
					m_Collision[i].type = Collision::eBox;
					m_Collision[i].box.center = m_tParam[i].pos;
					m_Collision[i].box.size = m_tParam[i].size;

					Collision::Result result = Collision::Hit(ballCollision, m_Collision[i]);
					if (result.isHit)
					{
						nHoldNo = i;
						m_bHold = true;
						pBallCount->SetEndInplay(CBallCount::InplayElement::HoldBall, true);
						if (pBall->GetIsFry())
						{
							CRunning::SetOut(CRunning::RunnerKind::BatterRunner, true);
						}
					}
				}
			}
			else
			{
				if (IsKeyPress('1'))Throwing(BaseKind::First);
				if (IsKeyPress('2'))Throwing(BaseKind::Second);
				if (IsKeyPress('3'))Throwing(BaseKind::Third);
				if (IsKeyPress('4'))Throwing(BaseKind::Home);
			}
		}
		break;
	default:
		break;
	}
}

void CFielding::Draw()
{
	for (int i = 0; i < (int)FieldMember::Max; i++)
	{

		if (CBall::GetInstance()->GetPhase() == BallPhase::Batting && i == (int)FieldMember::Chatcher)continue;

		SetModel(m_tParam[i],m_pFieldMember[i].get());
		Collision::DrawCollision(m_Collision[i]);
	}
}

void CFielding::SetModel(ModelParam param, Model* model, bool isAnime)
{
	SetRender3D();

	CCamera* pCamera = CCamera::GetInstance(CCamera::GetCameraKind()).get();

	param.mWorld =
		DirectX::XMMatrixScaling(param.size.x, param.size.y, param.size.z) *
		DirectX::XMMatrixRotationX(param.rotate.x) *
		DirectX::XMMatrixRotationY(param.rotate.y) *
		DirectX::XMMatrixRotationZ(param.rotate.z) *
		DirectX::XMMatrixTranslation(param.pos.x, param.pos.y, param.pos.z);

	DirectX::XMStoreFloat4x4(&param.wvp[0], DirectX::XMMatrixTranspose(param.mWorld));
	param.wvp[1] = pCamera->GetViewMatrix();		// view行列
	param.wvp[2] = pCamera->GetProjectionMatrix();	// projection行列
	// カメラ行列を設定
	Geometory::SetView(param.wvp[1]);
	Geometory::SetProjection(param.wvp[2]);


	// シェーダーへ変換行列を設定
	ShaderList::SetWVP(param.wvp); // 引数にはXMFloat4X4型の、要素数3のアドレスを渡すこと

	//モデルに使用する頂点シェーダーを設定
	model->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
	// モデルに使用する頂点ピクセルシェーダーを設定
	model->SetPixelShader(ShaderList::GetPS(ShaderList::PS_LAMBERT));

	for (int i = 0; i < model->GetMeshNum(); i++)
	{
		// モデルのメッシュの取得
		Model::Mesh mesh = *model->GetMesh(i);

		// メッシュに割り当てられているマテリアルを取得
		Model::Material material = *model->GetMaterial(mesh.materialID);

		// シェーダーへマテリアルを設定
		ShaderList::SetMaterial(material);

		// モデルの描画
		model->Draw(i);
	}
}

void CFielding::BaseCover()
{
	CField* pField = CField::GetInstance().get();
	for (int i = 0; i < (int)BaseKind::Max; i++)
	{
		m_bBaseCovered[i] = false;
		DirectX::XMFLOAT3 fBasePos = pField->GetBasePos((BaseKind)i);
		DirectX::XMFLOAT3 fBaseSize = pField->GetBaseSize((BaseKind)i);
		Collision::Info2D member;
		Collision::Info2D base;
		member.type = Collision::eSquare;
		member.square.pos = { m_tParam[m_nBaseNearNo[i]].pos.x,m_tParam[m_nBaseNearNo[i]].pos.z };
		member.square.size = { m_tParam[m_nBaseNearNo[i]].size.x,m_tParam[m_nBaseNearNo[i]].size.z };
		base.type = Collision::eSquare;
		base.square.pos = { fBasePos.x,fBasePos.z };
		base.square.size = { fBaseSize.x,fBaseSize.z };
		if (Collision::Hit2D(member, base).isHit)
		{
			m_bBaseCovered[i] = true;
			continue;
		}
		DirectX::XMVECTOR vecBasePos = DirectX::XMLoadFloat3(&fBasePos);
		DirectX::XMVECTOR vecMemberPos = DirectX::XMLoadFloat3(&m_tParam[m_nBaseNearNo[i]].pos);
		DirectX::XMVECTOR vecDirection = DirectX::XMVectorSubtract(vecBasePos, vecMemberPos);
		vecDirection = DirectX::XMVector3Normalize(vecDirection);
		vecDirection = DirectX::XMVectorScale(vecDirection, ce_fDifencePower);
		DirectX::XMFLOAT3 fMove;
		DirectX::XMStoreFloat3(&fMove, vecDirection);
		m_tParam[m_nBaseNearNo[i]].pos.x += fMove.x;
		m_tParam[m_nBaseNearNo[i]].pos.z += fMove.z;
	}
}

void CFielding::Throwing(BaseKind kind)
{
	CBall* pBall = CBall::GetInstance().get();
	CField* pField = CField::GetInstance().get();
	
	DirectX::XMFLOAT3 fBasePos = pField->GetBasePos(kind);
	DirectX::XMFLOAT3 fBallPos = pBall->GetPos();
	DirectX::XMVECTOR vecBallPos = DirectX::XMLoadFloat3(&fBallPos);
	DirectX::XMVECTOR vecBasePos = DirectX::XMLoadFloat3(&fBasePos);
	DirectX::XMVECTOR vecDirection = DirectX::XMVectorSubtract(vecBasePos, vecBallPos);
	DirectX::XMVECTOR vecThrowLength =  DirectX::XMVector3Length(vecDirection);
	float fThrowAngle = DirectX::XMVectorGetX(vecThrowLength);
	fThrowAngle /= 400.0f;

	vecDirection = DirectX::XMVector3Normalize(vecDirection);
	vecBallPos = DirectX::XMVectorAdd(vecBallPos, DirectX::XMVectorScale(vecDirection, 5.0f));
	DirectX::XMFLOAT3 fNewBallPos;
	DirectX::XMStoreFloat3(&fNewBallPos, vecBallPos);
	pBall->SetPos(fNewBallPos);
	vecDirection = DirectX::XMVectorScale(vecDirection, ce_fThrowingPower);
	DirectX::XMStoreFloat3(&m_fThrowDirection, vecDirection);

	m_fThrowDirection.y = fThrowAngle;
	pBall->SetMove(m_fThrowDirection);
	m_bHold = false;
}

int CFielding::OperationSearch()
{
	DirectX::XMFLOAT3 fBallPos = CBall::GetInstance()->GetPos();
	DirectX::XMVECTOR vecBallPos = DirectX::XMLoadFloat3(&fBallPos);
	DirectX::XMVECTOR vecMostNear = DirectX::XMVectorSet(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
	int nMostNearNo = -1;

	for (int i = 0; i < (int)FieldMember::Max; i++)
	{
		DirectX::XMVECTOR vecMemberPos = DirectX::XMLoadFloat3(&m_tParam[i].pos);
		DirectX::XMVECTOR vecDistance = DirectX::XMVectorSubtract(vecBallPos, vecMemberPos);
		vecDistance = DirectX::XMVector3Length(vecDistance);

		if (DirectX::XMVector3Less(vecDistance, vecMostNear) || i == 0)
		{
			vecMostNear = vecDistance;
			nMostNearNo = i;
		}
	}
	return nMostNearNo;
}

int CFielding::BaseSearch(BaseKind kind)
{
	DirectX::XMFLOAT3 fBasePos = CField ::GetInstance()->GetBasePos(kind);
	DirectX::XMVECTOR vecBasePos = DirectX::XMLoadFloat3(&fBasePos);
	DirectX::XMVECTOR vecMostNear = DirectX::XMVectorSet(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
	int nMostNearNo = -1;

	for (int i = 0; i < (int)FieldMember::Max; i++)
	{
		DirectX::XMVECTOR vecMemberPos = DirectX::XMLoadFloat3(&m_tParam[i].pos);
		DirectX::XMVECTOR vecDistance = DirectX::XMVectorSubtract(vecBasePos, vecMemberPos);
		vecDistance = DirectX::XMVector3Length(vecDistance);

		if (i == m_nOperationNo)continue;
		for (int j = 0; j < (int)BaseKind::Max; j++)
		{
			if (i == m_nBaseNearNo[j])continue;
		}

		if (DirectX::XMVector3Less(vecDistance, vecMostNear))
		{
			vecMostNear = vecDistance;
			nMostNearNo = i;
		}
	}
	return nMostNearNo;
}

Collision::Info CFielding::GetCollision(FieldMember Member)
{
	return Collision::Info();
}

DirectX::XMFLOAT3 CFielding::GetThrowDirection()
{
	return m_fThrowDirection;
}

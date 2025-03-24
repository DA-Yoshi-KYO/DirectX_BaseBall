#include "Fielding.h"
#include "Camera.h"
#include "Main.h"
#include "Ball.h"
#include "Field.h"
#include "Input.h"

CFielding::CFielding()
{
	for (int i = 0; i < (int)FieldMember::Max; i++)
	{
		m_pFieldMember[i] = std::make_unique<Model>();
		m_tParam[i].pos = { WORLD_AJUST,WORLD_AJUST, WORLD_AJUST };
		m_tParam[i].size = { 10.0f,10.0f,10.0f };
		m_tParam[i].rotate = { 0.0f,0.0f,0.0f };
	}
	m_pFieldMember[(int)FieldMember::Pitcher]->Load("ball.obj");
	m_pFieldMember[(int)FieldMember::Chatcher]->Load("ball.obj");
	m_pFieldMember[(int)FieldMember::First]->Load("ball.obj");
	m_pFieldMember[(int)FieldMember::Second]->Load("ball.obj");
	m_pFieldMember[(int)FieldMember::Third]->Load("ball.obj");
	m_pFieldMember[(int)FieldMember::Short]->Load("ball.obj");
	m_pFieldMember[(int)FieldMember::Left]->Load("ball.obj");
	m_pFieldMember[(int)FieldMember::Center]->Load("ball.obj");
	m_pFieldMember[(int)FieldMember::Right]->Load("ball.obj");
}

CFielding::~CFielding()
{
}

void CFielding::Update()
{
	DirectX::XMFLOAT3 fFieldPos = CField::GetInstance()->GetPos();
	DirectX::XMFLOAT3 fFieldSize = CField::GetInstance()->GetSize();
	DirectX::XMFLOAT3 fFieldPosLine = {fFieldSize.x / 10.0f,fFieldSize.y / 10.0f ,fFieldSize.z / 10.0f };
	int nNo = 0;

	switch (CBall::GetInstance()->GetPhase())
	{
	case BallPhase::Batting:
		m_tParam[(int)FieldMember::Pitcher].pos = { fFieldPos.x,fFieldPos.y, fFieldPos.z };
		m_tParam[(int)FieldMember::Chatcher].pos = { fFieldPos.x,fFieldPos.y, fFieldPos.z + fFieldPosLine.z * 3.0f };
		m_tParam[(int)FieldMember::First].pos = { fFieldPos.x - fFieldPosLine.x * 3.0f,fFieldPos.y, fFieldPos.z };
		m_tParam[(int)FieldMember::Second].pos = { fFieldPos.x - fFieldPosLine.x * 1.5f,fFieldPos.y, fFieldPos.z - fFieldPosLine.z * 1.5f };
		m_tParam[(int)FieldMember::Third].pos = { fFieldPos.x + fFieldPosLine.x * 3.0f,fFieldPos.y, fFieldPos.z };
		m_tParam[(int)FieldMember::Short].pos = { fFieldPos.x + fFieldPosLine.x * 1.5f,fFieldPos.y, fFieldPos.z - fFieldPosLine.z * 1.5f };
		m_tParam[(int)FieldMember::Left].pos = { fFieldPos.x + fFieldPosLine.x * 1.5f,fFieldPos.y, fFieldPos.z + fFieldPosLine.z * 1.5f };
		m_tParam[(int)FieldMember::Center].pos = { fFieldPos.x,fFieldPos.y, fFieldPos.z + fFieldPos.z + fFieldPosLine.z * 2.0f };
		m_tParam[(int)FieldMember::Right].pos = { fFieldPos.x - fFieldPosLine.x * 1.5f,fFieldPos.y, fFieldPos.z + fFieldPosLine.z * 1.5f };
		break;
	case BallPhase::InPlay:
		nNo = OperationSearch();

		if (IsKeyPress('W')) m_tParam[nNo].pos.z--;
		if (IsKeyPress('D')) m_tParam[nNo].pos.z++;
		if (IsKeyPress('S')) m_tParam[nNo].pos.x--;
		if (IsKeyPress('A')) m_tParam[nNo].pos.x++;
		break;
	default:
		break;
	}
}

void CFielding::Draw()
{
	for (int i = 0; i < (int)FieldMember::Max; i++)
	{
		SetModel(m_tParam[(int)FieldMember::Pitcher].pos, m_tParam[(int)FieldMember::Pitcher].size, m_tParam[(int)FieldMember::Pitcher].rotate, i);
	}
}

void CFielding::SetModel(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 rotate, int ModelType)
{
	CCamera* pCamera = CCamera::GetInstance(CCamera::GetCameraKind()).get();
	SetRender3D();


	m_tParam[ModelType].mWorld =
		DirectX::XMMatrixScaling(m_tParam[ModelType].size.x, m_tParam[ModelType].size.y, m_tParam[ModelType].size.z) *
		DirectX::XMMatrixRotationX(m_tParam[ModelType].rotate.x) *
		DirectX::XMMatrixRotationY(m_tParam[ModelType].rotate.y) *
		DirectX::XMMatrixRotationZ(m_tParam[ModelType].rotate.z) *
		DirectX::XMMatrixTranslation(m_tParam[ModelType].pos.x, m_tParam[ModelType].pos.y, m_tParam[ModelType].pos.z);

	DirectX::XMStoreFloat4x4(&m_tParam[ModelType].wvp[0], DirectX::XMMatrixTranspose(m_tParam[ModelType].mWorld));
	m_tParam[ModelType].wvp[1] = pCamera->GetViewMatrix();		// view行列
	m_tParam[ModelType].wvp[2] = pCamera->GetProjectionMatrix();	// projection行列
	// カメラ行列を設定
	Geometory::SetView(m_tParam[ModelType].wvp[1]);
	Geometory::SetProjection(m_tParam[ModelType].wvp[2]);


	// シェーダーへ変換行列を設定
	ShaderList::SetWVP(m_tParam[ModelType].wvp); // 引数にはXMFloat4X4型の、要素数3のアドレスを渡すこと

	//モデルに使用する頂点シェーダーを設定
	m_pFieldMember[ModelType]->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
	// モデルに使用する頂点ピクセルシェーダーを設定
	m_pFieldMember[ModelType]->SetPixelShader(ShaderList::GetPS(ShaderList::PS_LAMBERT));

	for (int i = 0; i < m_pFieldMember[ModelType]->GetMeshNum(); i++)
	{
		// モデルのメッシュの取得
		Model::Mesh mesh = *m_pFieldMember[ModelType]->GetMesh(i);

		// メッシュに割り当てられているマテリアルを取得
		Model::Material material = *m_pFieldMember[ModelType]->GetMaterial(mesh.materialID);

		// シェーダーへマテリアルを設定
		ShaderList::SetMaterial(material);

		// モデルの描画
		m_pFieldMember[ModelType]->Draw(i);
	}
}

int CFielding::OperationSearch()
{
	DirectX::XMFLOAT3 fBallPos = CBall::GetInstance()->GetPos();
	DirectX::XMVECTOR vecBallPos = DirectX::XMLoadFloat3(&fBallPos);
	DirectX::XMVECTOR vecMostNear = {};
	int nMostNearNo = 0;

	for (int i = 0; i < (int)FieldMember::Max; i++)
	{
		DirectX::XMVECTOR vecMemberPos = DirectX::XMLoadFloat3(&m_tParam[i].pos);
		DirectX::XMVECTOR vecDistance = DirectX::XMVectorSubtract(vecBallPos, vecMemberPos);
		vecDistance = DirectX::XMVector3Length(vecDistance);

		if (DirectX::XMVector3GreaterOrEqual(vecMostNear, vecDistance) || i == 0)
		{
			vecMostNear = vecDistance;
			nMostNearNo = i;
		}
	}
	return nMostNearNo;
}

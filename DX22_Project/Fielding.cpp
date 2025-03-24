#include "Fielding.h"
#include "Camera.h"
#include "Main.h"
#include "Ball.h"
#include "Field.h"
#include "Input.h"

CFielding::CFielding()
	: m_bHold(false)
{
	for (int i = 0; i < (int)FieldMember::Max; i++)
	{
		m_pFieldMember[i] = std::make_unique<Model>();
		m_tParam[i].pos = { WORLD_AJUST,WORLD_AJUST, WORLD_AJUST };
		m_tParam[i].size = { 5.0f,5.0f,5.0f };
		m_tParam[i].rotate = { 0.0f,0.0f,0.0f };
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
	DirectX::XMFLOAT3 fFieldPos = CField::GetInstance()->GetPos();
	fFieldPos.y = WORLD_AJUST + 10.0f;
	DirectX::XMFLOAT3 fFieldSizeMin = CField::GetInstance()->GetSize();
	DirectX::XMFLOAT3 fFieldSize = { fFieldSizeMin.x * 7.0f , fFieldSizeMin.y * 8.0f , fFieldSizeMin.z * 8.0f };

	DirectX::XMFLOAT3 fFieldPosLine = {fFieldSize.x / 10.0f,fFieldSize.y / 10.0f ,fFieldSize.z / 10.0f };
	int nNo = 0;

	switch (CBall::GetInstance()->GetPhase())
	{
	case BallPhase::Batting:
		m_tParam[(int)FieldMember::Pitcher].pos = { fFieldPos.x,fFieldPos.y, fFieldPos.z + fFieldPosLine.z * 1.75f };
		m_tParam[(int)FieldMember::Chatcher].pos = { fFieldPos.x,fFieldPos.y, fFieldPos.z + fFieldPosLine.z * 4.3f };
		m_tParam[(int)FieldMember::First].pos = { fFieldPos.x - fFieldPosLine.x * 1.9f,fFieldPos.y, fFieldPos.z + fFieldPosLine.z * 1.75f };
		m_tParam[(int)FieldMember::Second].pos = { fFieldPos.x - fFieldPosLine.x * 0.9f,fFieldPos.y, fFieldPos.z + fFieldPosLine.z * 0.6f };
		m_tParam[(int)FieldMember::Third].pos = { fFieldPos.x + fFieldPosLine.x * 1.9f,fFieldPos.y, fFieldPos.z + fFieldPosLine.z * 1.75f };
		m_tParam[(int)FieldMember::Short].pos = { fFieldPos.x + fFieldPosLine.x * 0.9f,fFieldPos.y, fFieldPos.z + fFieldPosLine.z * 0.6f };
		m_tParam[(int)FieldMember::Left].pos = { fFieldPos.x + fFieldPosLine.x * 2.3f,fFieldPos.y, fFieldPos.z - fFieldPosLine.z * 1.3f };
		m_tParam[(int)FieldMember::Center].pos = { fFieldPos.x,fFieldPos.y, fFieldPos.z - fFieldPosLine.z * 2.0f };
		m_tParam[(int)FieldMember::Right].pos = { fFieldPos.x - fFieldPosLine.x * 2.3f,fFieldPos.y, fFieldPos.z - fFieldPosLine.z * 1.3f };
		break;
	case BallPhase::InPlay:
		nNo = OperationSearch();

		if (IsKeyPress('W')) m_tParam[nNo].pos.z--;
		if (IsKeyPress('S')) m_tParam[nNo].pos.z++;
		if (IsKeyPress('D')) m_tParam[nNo].pos.x--;
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
		SetModel(m_tParam[i],m_pFieldMember[i].get());
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

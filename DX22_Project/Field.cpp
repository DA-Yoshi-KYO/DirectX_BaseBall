#include "Field.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "Main.h"

constexpr float ce_fFenceY = ce_fGroundY + 100.0f;
constexpr float ce_fStartEndZ = -85.0f;
constexpr float ce_fAjustZ = 98.0f;
constexpr float ce_fSinAjust = 0.2f;

CField::CField()
	: m_pField(nullptr),m_pBase(nullptr), m_pHomeBase(nullptr)
{
	InitModel();

	InitCollision();
}

CField::~CField()
{

}

void CField::Update()
{
	CBall* pBall = CBall::GetInstance().get();
	Collision::Info ballCollision = pBall->GetLineCollision();
	if (CCamera::GetCameraKind() == CameraKind::CAM_INPLAY)
	{
		for (int i = 0; i < m_HomeRunZone.size(); i++)
		{
			Collision::Result result = Collision::Hit(ballCollision.line, m_HomeRunZone[i].triangle);
			if (result.isHit)
			{
				result.other = m_HomeRunZone[i];
				pBall->OnCollision(result);
			}
		}
	}
}

void CField::Draw()
{
	SetModel(m_tFieldParam, m_pField.get());

	SetModel(m_tBaseParam[(int)BaseKind::Home], m_pHomeBase.get());
	for (int i = (int)BaseKind::First; i < (int)BaseKind::Third; i++)
	{
		SetModel(m_tBaseParam[i], m_pBase.get());
	}

	Collision::DrawCollision(m_Ground);
	for (auto itr = m_HomeRunZone.begin(); itr != m_HomeRunZone.end(); itr++)
	{
		Collision::DrawCollision((*itr));
	}
}

void CField::SetModel(ModelParam param, Model* model, bool isAnime)
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

	for (UINT i = 0; i < model->GetMeshNum(); i++)
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

void CField::OnCollision(Collision::Result collision)
{

}

std::unique_ptr<CField>& CField::GetInstance()
{
	static std::unique_ptr<CField> instance(new CField());
	return instance;
}

DirectX::XMFLOAT3 CField::GetPos()
{
	return m_tFieldParam.pos;
}

DirectX::XMFLOAT3 CField::GetBasePos(BaseKind No)
{
	return m_tBaseParam[(int)No].pos;
}

DirectX::XMFLOAT3 CField::GetBaseSize(BaseKind No)
{
	return m_tBaseParam[(int)No].size;
}

DirectX::XMFLOAT3 CField::GetSize()
{
	return m_tFieldParam.size;
}

DirectX::XMFLOAT3 CField::GetRotation()
{
	return m_tFieldParam.rotate;
}

void CField::InitModel()
{
	m_pField = std::make_unique<Model>();
	m_pField->Load(MODELPASS("BaseBallPark.fbx"));

	m_tFieldParam.pos = { 0.0f + WORLD_AJUST,-10.0f + WORLD_AJUST,0.0f + WORLD_AJUST  - 150.0f };
	m_tFieldParam.size = { 50.0f,50.0f,50.0f };
	m_tFieldParam.rotate = { 0.0f,0.0f,0.0f };

	m_pBase = std::make_unique<Model>();
	if(!m_pBase->Load(MODELPASS("base.obj"))) ERROR_MESSAGE("base.obj");
	m_pHomeBase = std::make_unique<Model>();
	if(!m_pHomeBase->Load(MODELPASS("HomeBase.obj")))ERROR_MESSAGE("HomeBase.obj");

	m_tBaseParam[(int)BaseKind::Home].pos = { ce_fPitcherPos.x, m_tFieldParam.pos.y,ce_fPitcherPos.z + 75.0f };
	m_tBaseParam[(int)BaseKind::Home].size = { 1.5f,0.5f,1.5f };
	m_tBaseParam[(int)BaseKind::Home].rotate = { 0.0f,0.0f,0.0f };

	for (int i = (int)BaseKind::First; i < (int)BaseKind::Third; i++)
	{
		m_tBaseParam[i].size = { 3.0f,3.0f,3.0f };
		m_tBaseParam[i].rotate = { 0.0f,DirectX::XMConvertToRadians(45.0f),0.0f};
	}
	m_tBaseParam[(int)BaseKind::First].pos = { ce_fPitcherPos.x - 75.0f,m_tFieldParam.pos.y,ce_fPitcherPos.z - 35.0f};
	m_tBaseParam[(int)BaseKind::Second].pos = { ce_fPitcherPos.x ,m_tFieldParam.pos.y,ce_fPitcherPos.z - 115.0f };
	m_tBaseParam[(int)BaseKind::Third].pos = { ce_fPitcherPos.x + 75.0f,m_tFieldParam.pos.y,ce_fPitcherPos.z - 35.0f };
}

void CField::InitCollision()
{
	// 当たり判定初期化
	// グラウンド
	m_Ground.type = Collision::Type::eBox;
	m_Ground.box.center = m_tFieldParam.pos;
	m_Ground.box.size = { m_tFieldParam.size.x * 8.0f,m_tFieldParam.size.y,m_tFieldParam.size.z * 8.0f };

	// ホームランフェンス・ホームランゾーン
	int i = 0;
	int j = 0;
	m_HomeRunZone.clear();
	m_HomeRunZone.resize(ce_nMaxHomerunPolyLine);
	float fStep = 609.0f / (m_HomeRunZone.size() - 1);
	float fBaseX = WORLD_AJUST + 160.0f;

	for (auto itr = m_HomeRunZone.begin(); itr != m_HomeRunZone.end(); itr++, i++)
	{
		float fAngleZ = 0;

		(*itr).type = Collision::eTriangle;


		if (i % 2 == 0)
		{
			// 0,1
			fAngleZ = DirectX::XMConvertToRadians(360.0f) / m_HomeRunZone.size() * (i / 2);
			fAngleZ = -sinf(fAngleZ);

			(*itr).triangle.point[0] = { fBaseX - fStep * (i / 2), ce_fFenceY + WORLD_AJUST, fAngleZ * ce_fAjustZ + WORLD_AJUST + ce_fStartEndZ };
			(*itr).triangle.point[1] = { fBaseX - fStep * (i / 2), ce_fGroundY + WORLD_AJUST, fAngleZ * ce_fAjustZ + WORLD_AJUST + ce_fStartEndZ };

			// 2
			fAngleZ = DirectX::XMConvertToRadians(360.0f) / m_HomeRunZone.size() * (i / 2 + 1);
			fAngleZ = -sinf(fAngleZ);
			(*itr).triangle.point[2] = { fBaseX - fStep * (i / 2 + 1), ce_fFenceY + WORLD_AJUST, fAngleZ * ce_fAjustZ + WORLD_AJUST + ce_fStartEndZ };
		}
		else
		{
			// 2
			fAngleZ = DirectX::XMConvertToRadians(360.0f) / m_HomeRunZone.size() * (i / 2);
			fAngleZ = -sinf(fAngleZ);

			(*itr).triangle.point[0] = { fBaseX - fStep * (i / 2), ce_fGroundY + WORLD_AJUST, fAngleZ * ce_fAjustZ + WORLD_AJUST + ce_fStartEndZ };

			// 1,3
			fAngleZ = DirectX::XMConvertToRadians(360.0f) / m_HomeRunZone.size() * (i / 2 + 1);
			fAngleZ = -sinf(fAngleZ);
			(*itr).triangle.point[1] = { fBaseX - fStep * (i / 2 + 1), ce_fFenceY + WORLD_AJUST, fAngleZ * ce_fAjustZ + WORLD_AJUST + ce_fStartEndZ };
			(*itr).triangle.point[2] = { fBaseX - fStep * (i / 2 + 1), ce_fGroundY + WORLD_AJUST, fAngleZ * ce_fAjustZ + WORLD_AJUST + ce_fStartEndZ };

			j += 1;  // 1ずつ増やして連続性を保つ
		}
	}
}

#include "Field.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "Main.h"

constexpr float ce_fFenceY = ce_fGroundY + 100.0f;
constexpr float ce_fStartEndZ = -85.0f;
constexpr float ce_fAjustZ = 98.0f;
constexpr float ce_fSinAjust = 0.2f;

CField::CField()
	: m_pField(nullptr)
{
	m_pField = std::make_unique<Model>();
	m_pField->Load(MODELPASS("BaseBallPark.fbx"));

	m_tFieldParam.pos = { 0.0f + WORLD_AJUST,-10.0f + WORLD_AJUST,0.0f + WORLD_AJUST };
	m_tFieldParam.size = { 50.0f,50.0f,50.0f };
	m_tFieldParam.rotate = { 0.0f,0.0f,0.0f };

	m_Ground.type = Collision::Type::eBox;
	m_Ground.box.center = m_tFieldParam.pos;
	m_Ground.box.size = { m_tFieldParam.size.x * 8.0f,m_tFieldParam.size.y,m_tFieldParam.size.z * 8.0f };

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

CField::~CField()
{

}

void CField::Update()
{
	CBall* pBall = CBall::GetInstance().get();
	Collision::Info ballCollision = pBall->GetCollision();
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
	SetRender3D();
	
	CCamera* pCamera = CCamera::GetInstance(CCamera::GetCameraKind()).get();

	m_tFieldParam.mWorld =
		DirectX::XMMatrixScaling(m_tFieldParam.size.x, m_tFieldParam.size.y, m_tFieldParam.size.z) *
		DirectX::XMMatrixRotationX(m_tFieldParam.rotate.x) *
		DirectX::XMMatrixRotationY(m_tFieldParam.rotate.y) *
		DirectX::XMMatrixRotationZ(m_tFieldParam.rotate.z) *
		DirectX::XMMatrixTranslation(m_tFieldParam.pos.x, m_tFieldParam.pos.y, m_tFieldParam.pos.z);

	DirectX::XMStoreFloat4x4(&m_tFieldParam.wvp[0], DirectX::XMMatrixTranspose(m_tFieldParam.mWorld));
	m_tFieldParam.wvp[1] = pCamera->GetViewMatrix();		// view行列
	m_tFieldParam.wvp[2] = pCamera->GetProjectionMatrix();	// projection行列
	// カメラ行列を設定
	Geometory::SetView(m_tFieldParam.wvp[1]);
	Geometory::SetProjection(m_tFieldParam.wvp[2]);


	// シェーダーへ変換行列を設定
	ShaderList::SetWVP(m_tFieldParam.wvp); // 引数にはXMFloat4X4型の、要素数3のアドレスを渡すこと

	//モデルに使用する頂点シェーダーを設定
	m_pField->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
	// モデルに使用する頂点ピクセルシェーダーを設定
	m_pField->SetPixelShader(ShaderList::GetPS(ShaderList::PS_LAMBERT));

	for (int i = 0; i < m_pField->GetMeshNum(); i++)
	{
		// モデルのメッシュの取得
		Model::Mesh mesh = *m_pField->GetMesh(i);

		// メッシュに割り当てられているマテリアルを取得
		Model::Material material = *m_pField->GetMaterial(mesh.materialID);

		// シェーダーへマテリアルを設定
		ShaderList::SetMaterial(material);

		// モデルの描画
		m_pField->Draw(i);
	}

	Collision::DrawCollision(m_Ground);
	for (auto itr = m_HomeRunZone.begin(); itr != m_HomeRunZone.end(); itr++)
	{
		Collision::DrawCollision((*itr));
	}
}

void CField::SetModel(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 rotate, int ModelType)
{

}

void CField::OnCollision(Collision::Result collision)
{

}

Collision::Info CField::GetCollision()
{
	return m_Collision;
}

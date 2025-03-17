#include "Field.h"

constexpr float FIELD_SIZE_POW = 20.0f;

CField::CField()
	: m_pCamera(nullptr), m_pField(nullptr)
{
	m_pField = std::make_unique<Model>();
	m_pField->Load(MODELPASS("BaseBallPark.fbx"));

	m_tFieldParam.pos = { 0.0f,0.0f,0.0f };
	m_tFieldParam.size = { 50.0f,1.0f,50.0f };
	m_tFieldParam.rotate = { 0.0f,0.0f,0.0f };
}

CField::~CField()
{
	m_pCamera.release();
}

void CField::Update()
{
}

void CField::Draw()
{
	m_tFieldParam.mWorld =
		DirectX::XMMatrixScaling(m_tFieldParam.size.x, m_tFieldParam.size.y, m_tFieldParam.size.z) *
		DirectX::XMMatrixRotationX(m_tFieldParam.rotate.x) *
		DirectX::XMMatrixRotationY(m_tFieldParam.rotate.y) *
		DirectX::XMMatrixRotationZ(m_tFieldParam.rotate.z) *
		DirectX::XMMatrixTranslation(m_tFieldParam.pos.x + WORLD_AJUST, m_tFieldParam.pos.y + WORLD_AJUST, m_tFieldParam.pos.z + WORLD_AJUST);

	DirectX::XMStoreFloat4x4(&m_tFieldParam.wvp[0], DirectX::XMMatrixTranspose(m_tFieldParam.mWorld));
	m_tFieldParam.wvp[1] = m_pCamera->GetViewMatrix();		// view行列
	m_tFieldParam.wvp[2] = m_pCamera->GetProjectionMatrix();	// projection行列
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
}

void CField::SetModel(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 rotate, int ModelType)
{

}

void CField::SetCamera(CCamera* camera)
{
	m_pCamera.reset(camera);
}	

void CField::OnCollision(Collision::Result collision)
{

}

Collision::Info CField::GetCollision()
{
	return m_Collision;
}

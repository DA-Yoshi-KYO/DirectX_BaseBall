#include "Ball.h"

constexpr DirectX::XMFLOAT3 BALL_POS = { 0.0f,3.0f,180.0f };
constexpr float BALL_SIZE = 3.0f;

enum class BallPhase
{
	Start,
	Have,
	Release,
	Scattered
};

CBall::CBall()
	: m_pModel(nullptr), m_pPlayer(nullptr), m_pCamera(nullptr)
	, m_nBallPhase((int)BallPhase::Start)
	, m_bHave(false), m_fRandX(0.0f)
{
	// ボールのモデルの読み込み
	m_pModel = new Model();
	if (!m_pModel->Load(PASS("ball.fbx"))) MessageBox(NULL, "tree_desert.fbx", "Error", MB_OK);	// エラーメッセージの表示
	m_pos = { BALL_POS.x + WORLD_AJUST ,BALL_POS.y + WORLD_AJUST, BALL_POS.z + WORLD_AJUST };
	m_size = { BALL_SIZE,BALL_SIZE, BALL_SIZE };
	m_Collision.type = Collision::eBox;
	m_Collision.box = { m_pos,m_size };
}

CBall::~CBall()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pPlayer);
}

void CBall::Update()
{
	switch (m_nBallPhase)
	{
	case (int)BallPhase::Start:
		m_pos.z -= 1.0f;
		m_pos.x += m_fRandX;
		break;
	case (int)BallPhase::Have:
		break;
	case (int)BallPhase::Release:
		break;
	case (int)BallPhase::Scattered:
		break;
	default:
		break;
	}
	m_Collision.box = { m_pos,m_size };
}

void CBall::Draw()
{
	if(m_nBallPhase != (int)BallPhase::Have)SetModel(m_pos, m_size, m_rotate);
}

void CBall::SetModel(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 rotate, int ModelType)
{
	// ワールド行列変換
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);	// 座標
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(size.z, size.z, size.z);		// 拡縮
	DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(rotate.z);				// 回転X
	DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(rotate.y);				// 回転Y
	DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(rotate.z);				// 回転Z
	DirectX::XMMATRIX world = S * Rx * Ry * Rz * T;	// 拡縮・回転・座標の順番でかけ合わせる

	DirectX::XMFLOAT4X4 wvp[3] = {};	// 各行列変換の受け入れ先

	// 計算用のデータから読み取り用のデータに変換(転置)
	DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(world));	// ワールド行列を転置して設定
	// view行列とprojection行列はカメラの物を持ってくる
	wvp[1] = m_pCamera->GetViewMatrix();		// view行列
	wvp[2] = m_pCamera->GetProjectionMatrix();	// projection行列
	// カメラ行列を設定
	Geometory::SetView(wvp[1]);
	Geometory::SetProjection(wvp[2]);

	// シェーダーへ変換行列を設定
	ShaderList::SetWVP(wvp); // 引数にはXMFloat4X4型の、要素数3のアドレスを渡すこと

	//モデルに使用する頂点シェーダーを設定
	m_pModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
	// モデルに使用する頂点ピクセルシェーダーを設定
	m_pModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_LAMBERT));

	for (int i = 0; i < m_pModel->GetMeshNum(); i++)
	{
		// モデルのメッシュの取得
		Model::Mesh mesh = *m_pModel->GetMesh(i);

		// メッシュに割り当てられているマテリアルを取得
		Model::Material material = *m_pModel->GetMaterial(mesh.materialID);

		material.ambient.x = 0.75f;
		material.ambient.y = 0.75f;
		material.ambient.z = 0.75f;

		// シェーダーへマテリアルを設定
		ShaderList::SetMaterial(material);

		// モデルの描画
		m_pModel->Draw(i);
	}
}

void CBall::OnCollision(Collision::Result collision)
{
	m_nBallPhase = (int)BallPhase::Have;
}

Collision::Info CBall::GetCollision()
{
	return m_Collision;
}

void CBall::SetCamera(CCamera* camera)
{
	m_pCamera = camera;
}

void CBall::SetPlayer(CPlayer* player)
{
	m_pPlayer = player;
}

bool CBall::IsHaving()
{
	return m_bHave;
}

void CBall::ReleaseBall()
{
	m_bHave = false;
}

void CBall::SetPos(DirectX::XMFLOAT3 pos)
{
	m_pos = pos;
}

DirectX::XMFLOAT3 CBall::GetPos()
{
	return m_pos;
}

int CBall::GetPhase()
{
	return m_nBallPhase;
}

void CBall::SetPhase(int nPhase)
{
	m_nBallPhase = nPhase;
}

void CBall::SetRandX()
{
	int n = -10;
	n += rand() % 20;
	float x = (float)n * 0.05f;

	m_fRandX = x;
}

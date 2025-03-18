#include "Ball.h"
#include "ImGuiManager.h"
#include "SceneGame.h"


constexpr DirectX::XMFLOAT2 ce_fBallEndCenter = { 0.0f,2.0f };
constexpr DirectX::XMFLOAT2 ce_fBallLimitX = { 2.7f,-2.7f };
constexpr DirectX::XMFLOAT2 ce_fBallLimitY = { -2.7f,2.3f };
constexpr  DirectX::XMFLOAT3 ce_fBallSize = { 10.0f,10.0f,10.0f };
constexpr int ce_nBallRotateSec = 220 / 60;

CBall::CBall()
	: m_pModel(nullptr), m_pCamera(nullptr), m_pPitching(nullptr)
{
	// ボールのモデルの読み込み
	m_pModel = std::make_unique<Model>();
	if (!m_pModel->Load(MODELPASS("ball.fbx"))) ERROR_MESSAGE("ball.fbx");

	m_pos = { ce_fBallPos.x + WORLD_AJUST ,ce_fBallPos.y + WORLD_AJUST, ce_fBallPos.z + WORLD_AJUST };
	m_size = ce_fBallSize;
	m_Collision.type = Collision::eBox;
	m_Collision.box = { m_pos,m_size };
}

CBall::~CBall()
{
	m_pCamera.release();
	m_pPitching.release();
	m_pCursor.release();
}

void CBall::Update()
{
	if (m_pPitching->GetPitchingPhase() == (int)CPitching::PitchingPhase::Release)
	{
		float fChatch = m_pPitching->GetChatchTime();
		DirectX::XMFLOAT2 fCursorPos = m_pCursor->GetPos((int)CSceneGame::Playing::Defence);
		DirectX::XMFLOAT2 fCenterToCursor = { ce_fCursorPos.x - fCursorPos.x,ce_fCursorPos.y - fCursorPos.y };
		DirectX::XMFLOAT2 fZoneSizeHarf = { ce_fStrikeZoneSize.x / 2.0f,ce_fStrikeZoneSize.y / 2.0f };
		DirectX::XMFLOAT2 fCenterToCursorPow = { fCenterToCursor.x / fZoneSizeHarf.x,fCenterToCursor.y / fZoneSizeHarf.y };
		DirectX::XMFLOAT2 fCenterToBall = { ce_fBallLimitX.x * fCenterToCursorPow.x,  ce_fBallLimitX.y * fCenterToCursorPow.y + ce_fBallEndPos.y };

		m_pos.x += (fCenterToBall.x - ce_fBallPos.x) / fChatch;
		m_pos.y += (fCenterToBall.y - ce_fBallPos.y) / fChatch;
		m_pos.z += (ce_fBallEndPos.z - ce_fBallPos.z) / fChatch;
		m_rotate.y += DirectX::XMConvertToRadians(ce_nBallRotateSec * 360.0f) / fChatch;
	}
	else
	{
		m_pos = { ce_fBallPos.x + WORLD_AJUST ,ce_fBallPos.y + WORLD_AJUST, ce_fBallPos.z + WORLD_AJUST };
	}
}

void CBall::Draw()
{
#ifdef _IMGUI
	DirectX::XMFLOAT3X3 debug = GetPosSizeRotateDebug("Ball");
#endif // _IMGUI
	if (m_pPitching->GetPitchingPhase() == (int)CPitching::PitchingPhase::Release)
	{
		SetModel(m_pos, m_size, m_rotate);
	}
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
}

Collision::Info CBall::GetCollision()
{
	return m_Collision;
}

void CBall::SetCamera(CCamera* camera)
{
	m_pCamera.reset(camera);
}

void CBall::SetPitching(CPitching* pitching)
{
	m_pPitching.reset(pitching);
}

void CBall::SetCursor(CCursor* cursor)
{
	m_pCursor.reset(cursor);
}

DirectX::XMFLOAT3 CBall::GetPos()
{
	return m_pos;
}

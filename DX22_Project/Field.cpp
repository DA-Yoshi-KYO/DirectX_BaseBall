// ==============================
//    インクルード部
// ==============================
#include "Field.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "Main.h"
#include "Fielding.h"

// ==============================
//    定数定義
// ==============================
constexpr int ce_nHomerunPolyLine = 40;	// ホームランゾーンのポリライン分割数
constexpr int ce_nPlanePolyLine = 2;	// 直線平面のポリライン分割数
constexpr float ce_nEasPow = 2.5f;	// イージングの強さ
constexpr float ce_fJudgeZoneY = ce_fGroundY + 100.0f;	// 判定用当たり判定の高さ
constexpr float ce_fStartEndZ = -110.0f;	// 外野フェンスポリラインの最初のZ値
constexpr float ce_fAjustZ = 200.0f;	// 外野フェンスの頂点(ce_fStartEndZから見た膨らみの最大値)
constexpr float ce_fFenceX = 500.0f;	// 外野フェンスのX距離
constexpr float ce_fHomeToBatterBoxX = 0.0f;	// バッターボックスまでの距離
constexpr float ce_fHomeToBatterBoxZ = 5.0f;	// バッターボックスまでの距離

CField::CField()
	: m_pField(nullptr),m_pBase(nullptr), m_pHomeBase(nullptr)
{
	// モデルの初期化
	InitModel();

	// 当たり判定情報の初期化
	InitCollision();
}

CField::~CField()
{

}

void CField::Update()
{
	CBall* pBall = CBall::GetInstance().get();
	Collision::Info ballCollision = pBall->GetLineCollision();

	// フィールドの処理はインプレー時に行う
	if (CCamera::GetCameraKind() == CameraKind::CAM_INPLAY)
	{
		// ホームランゾーンと外野フェンスの処理
		for (int i = 0; i < m_HomeRunZone.size(); i++)
		{
			Collision::Result result = Collision::Hit(ballCollision.line, m_HomeRunZone[i].triangle);
			if (result.isHit)
			{
				result.other = m_HomeRunZone[i];
				pBall->OnCollision(result);
			}
		}

		for (int i = 0; i < m_FirstBaseLine.size(); i++)
		{
			Collision::Result result = Collision::Hit(ballCollision.line, m_FirstBaseLine[i].triangle);
			if(CFielding::GetChatchPattern() != CFielding::ChatchPattern::Fry &&
				result.isHit)
			{
				pBall->OnFoulZone(result);
			}
		}
	}
}

void CField::Draw()
{
	// スカイドームの描画
	//SetModel(m_tSkydomeParam, m_pSkydome.get());

	// グラウンドの描画
	SetModel(m_tFieldParam, m_pField.get());

	// ベースの描画
	SetModel(m_tBaseParam[(int)BaseKind::Home], m_pHomeBase.get());
	for (int i = (int)BaseKind::First; i <= (int)BaseKind::Third; i++)
	{
		SetModel(m_tBaseParam[i], m_pBase.get());
	}

	//Collision::DrawCollision(m_Ground);
	for (auto itr = m_HomeRunZone.begin(); itr != m_HomeRunZone.end(); itr++)
	{
		Collision::DrawCollision((*itr));
	}
	for (auto itr = m_FirstBaseLine.begin(); itr != m_FirstBaseLine.end(); itr++)
	{
		Collision::DrawCollision((*itr));
	}
	for (auto itr = m_ThirdBaseLine.begin(); itr != m_ThirdBaseLine.end(); itr++)
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

void CField::InitModel()
{
	// モデルの読み込み
	m_pField = std::make_unique<Model>();
	if (!m_pField->Load(MODELPASS("Baseball_Ground.fbx"),0.1f)) ERROR_MESSAGE("Baseball_Ground.fbx");
	m_pSkydome = std::make_unique<Model>();
	if (!m_pSkydome->Load(MODELPASS("basic_skybox_3d_flip.fbx"))) ERROR_MESSAGE("basic_skybox_3d_flip.fbx");
	m_pBase = std::make_unique<Model>();
	if(!m_pBase->Load(MODELPASS("base.obj"))) ERROR_MESSAGE("base.obj");
	m_pHomeBase = std::make_unique<Model>();
	if(!m_pHomeBase->Load(MODELPASS("HomeBase.obj")))ERROR_MESSAGE("HomeBase.obj");

	// パラメータの初期化
	// グラウンド
	m_tFieldParam.pos = { 0.0f + WORLD_AJUST,-10.0f + WORLD_AJUST,0.0f + WORLD_AJUST -40.0f };
	m_tFieldParam.size = { 50.0f,50.0f,50.0f };
	m_tFieldParam.rotate = { 0.0f,0.0f,0.0f };

	m_tSkydomeParam.pos = { 0.0f + WORLD_AJUST,-10.0f + WORLD_AJUST,0.0f + WORLD_AJUST  - 150.0f };
	m_tSkydomeParam.size = { 500.0f,500.0f,500.0f };
	m_tSkydomeParam.rotate = { 0.0f,0.0f,0.0f };

	// 各種ベース
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
	int i = 0;	// ループ用(毎回)
	int j = 0;	// ループ用(2回に1回)
	m_HomeRunZone.clear();	// ベクターの初期化
	m_HomeRunZone.resize(ce_nHomerunPolyLine);	// ベクターのリサイズ
	const float fStep = ce_fFenceX * 2.0f / (m_HomeRunZone.size());	// フェンスx軸の大きさからポリライン一つあたりのステップを求める
	const float fBaseX = WORLD_AJUST + ce_fFenceX / 2.0f;	// フェンスx軸の左端

	float fEasValue[ce_nHomerunPolyLine / 2 + 1];

	for (int i = 0; i < ce_nHomerunPolyLine / 2 + 1; i++)
	{
		int nEasCount = (i) - m_HomeRunZone.size() / 4;
		nEasCount = abs(nEasCount);
		nEasCount = m_HomeRunZone.size() / 4 - nEasCount;
		float fT = (float)nEasCount / ((float)m_HomeRunZone.size() / 4);
		fEasValue[i] = powf(1 - fT, ce_nEasPow);
		fEasValue[i] += 1.0f;
	}

	// ポリライン分割数の数だけ初期化する
	for (auto itr = m_HomeRunZone.begin(); itr != m_HomeRunZone.end(); itr++, i++)
	{
		float fAngleZ = 0;	// ポリラインの角度
		(*itr).type = Collision::eTriangle;	// コリジョンの形は三角形

		// イージングを使って弧を急にする...？


		// 三角形を使用した当たり判定のイメージ
		//(0)  (2)
		//  *─*
		// │／│
		//  *─*
		//(1)  (3)

		// 三角形インデックスを指定し、四角形の当たり判定を扇状に設置する
		// 上三角形(インデックス:0,1,2)
		if (i % 2 == 0)
		{

			// 0,1
			// ポリラインの場所に応じて角度を決める
			fAngleZ = DirectX::XMConvertToRadians(360.0f) / m_HomeRunZone.size() * (i / 2);
			// 角度に応じてZ値をサインカーブで求める
			fAngleZ = -sinf(fAngleZ) * fEasValue[j];	// ここにイージング...?


			// 左側の頂点
			(*itr).triangle.point[0] = { fBaseX - fStep * (i / 2), ce_fJudgeZoneY + WORLD_AJUST, fAngleZ * ce_fAjustZ + WORLD_AJUST + ce_fStartEndZ };
			(*itr).triangle.point[1] = { fBaseX - fStep * (i / 2), ce_fGroundY + WORLD_AJUST, fAngleZ * ce_fAjustZ + WORLD_AJUST + ce_fStartEndZ };
			// 2
			// ポリラインの場所に応じて角度を決める
			fAngleZ = DirectX::XMConvertToRadians(360.0f) / m_HomeRunZone.size() * (i / 2 + 1);
			// 角度に応じてZ値をサインカーブで求める
			fAngleZ = -sinf(fAngleZ) * fEasValue[j + 1];

			// 右側の頂点
			(*itr).triangle.point[2] = { fBaseX - fStep * (i / 2 + 1), ce_fJudgeZoneY + WORLD_AJUST, fAngleZ * ce_fAjustZ + WORLD_AJUST + ce_fStartEndZ };
		}
		// 下三角形(1,2,3)
		else
		{
			// 1
			// ポリラインの場所に応じて角度を決める
			fAngleZ = DirectX::XMConvertToRadians(360.0f) / m_HomeRunZone.size() * (i / 2);
			// 角度に応じてZ値をサインカーブで求める
			fAngleZ = -sinf(fAngleZ) * fEasValue[j];

			// 左側の頂点
			(*itr).triangle.point[0] = { fBaseX - fStep * (i / 2), ce_fGroundY + WORLD_AJUST, fAngleZ * ce_fAjustZ + WORLD_AJUST + ce_fStartEndZ };

			// 2,3
			// ポリラインの場所に応じて角度を決める
			fAngleZ = DirectX::XMConvertToRadians(360.0f) / m_HomeRunZone.size() * (i / 2 + 1);
			// 角度に応じてZ値をサインカーブで求める
			fAngleZ = -sinf(fAngleZ) * fEasValue[j + 1];

			// 右側の頂点
			(*itr).triangle.point[1] = { fBaseX - fStep * (i / 2 + 1), ce_fJudgeZoneY + WORLD_AJUST, fAngleZ * ce_fAjustZ + WORLD_AJUST + ce_fStartEndZ };
			(*itr).triangle.point[2] = { fBaseX - fStep * (i / 2 + 1), ce_fGroundY + WORLD_AJUST, fAngleZ * ce_fAjustZ + WORLD_AJUST + ce_fStartEndZ };

			j += 1;
		}
	}

	m_FirstBaseLine.resize(ce_nPlanePolyLine);
	m_FirstBaseLine[0].type = Collision::eTriangle;
	m_FirstBaseLine[0].triangle.point[0] = { m_tBaseParam[(int)BaseKind::Home].pos.x + ce_fHomeToBatterBoxX  ,ce_fJudgeZoneY + WORLD_AJUST , m_tBaseParam[(int)BaseKind::Home].pos.z + ce_fHomeToBatterBoxZ };
	m_FirstBaseLine[0].triangle.point[1] = { m_tBaseParam[(int)BaseKind::Home].pos.x + ce_fHomeToBatterBoxX  ,ce_fGroundY + WORLD_AJUST , m_tBaseParam[(int)BaseKind::Home].pos.z + ce_fHomeToBatterBoxZ };
	m_FirstBaseLine[0].triangle.point[2] = { fBaseX , ce_fJudgeZoneY + WORLD_AJUST , ce_fStartEndZ + WORLD_AJUST };
	m_FirstBaseLine[1].type = Collision::eTriangle;
	m_FirstBaseLine[1].triangle.point[0] = { m_tBaseParam[(int)BaseKind::Home].pos.x + ce_fHomeToBatterBoxX  ,ce_fGroundY + WORLD_AJUST, m_tBaseParam[(int)BaseKind::Home].pos.z + ce_fHomeToBatterBoxZ };
	m_FirstBaseLine[1].triangle.point[1] = { fBaseX , ce_fJudgeZoneY + WORLD_AJUST , ce_fStartEndZ + WORLD_AJUST };
	m_FirstBaseLine[1].triangle.point[2] = { fBaseX , ce_fGroundY + WORLD_AJUST, ce_fStartEndZ + WORLD_AJUST };

	m_ThirdBaseLine.resize(ce_nPlanePolyLine);
	m_ThirdBaseLine[0].type = Collision::eTriangle;
	m_ThirdBaseLine[0].triangle.point[0] = { m_tBaseParam[(int)BaseKind::Home].pos.x - ce_fHomeToBatterBoxX ,ce_fJudgeZoneY + WORLD_AJUST , m_tBaseParam[(int)BaseKind::Home].pos.z + ce_fHomeToBatterBoxZ };
	m_ThirdBaseLine[0].triangle.point[1] = { m_tBaseParam[(int)BaseKind::Home].pos.x - ce_fHomeToBatterBoxX ,ce_fGroundY + WORLD_AJUST , m_tBaseParam[(int)BaseKind::Home].pos.z + ce_fHomeToBatterBoxZ };
	m_ThirdBaseLine[0].triangle.point[2] = { fBaseX - ce_fFenceX, ce_fJudgeZoneY + WORLD_AJUST , ce_fStartEndZ + WORLD_AJUST };
	m_ThirdBaseLine[1].type = Collision::eTriangle;
	m_ThirdBaseLine[1].triangle.point[0] = { m_tBaseParam[(int)BaseKind::Home].pos.x - ce_fHomeToBatterBoxX ,ce_fGroundY + WORLD_AJUST , m_tBaseParam[(int)BaseKind::Home].pos.z + ce_fHomeToBatterBoxZ };
	m_ThirdBaseLine[1].triangle.point[1] = { fBaseX - ce_fFenceX, ce_fJudgeZoneY + WORLD_AJUST , ce_fStartEndZ + WORLD_AJUST };
	m_ThirdBaseLine[1].triangle.point[2] = { fBaseX - ce_fFenceX, ce_fGroundY + WORLD_AJUST , ce_fStartEndZ + WORLD_AJUST };
}

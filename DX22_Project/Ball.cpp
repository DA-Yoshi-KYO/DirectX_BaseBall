#include "Ball.h"
#include "ImGuiManager.h"
#include "SceneGame.h"
#include "Sprite.h"
#include "Main.h"
#include "BallCount.h"
#include "Running.h"

constexpr DirectX::XMFLOAT2 ce_fBallLimitX = { 2.7f,-2.7f };
constexpr DirectX::XMFLOAT2 ce_fBallLimitY = { -2.7f,2.3f };
constexpr  DirectX::XMFLOAT3 ce_fBallSize = { 0.5f,0.5f,0.5f };
constexpr  DirectX::XMFLOAT3 ce_fInplayBallSize = { 2.0f,2.0f,2.0f };
constexpr int ce_nBallRotateSec = 220 / 60;

CBall::CBall()
	: m_pModel(nullptr), m_pPitching(nullptr)
	, m_nPhase((int)BallPhase::Batting)
	, m_fMove{}, m_fShadowPos{}
	, m_bFry(true), m_bBallFaulZone(false), m_fFaulZoneBallPos{}
	, m_fPitchPos{}, m_fPredValue{}
{
	// ボールのモデルの読み込み
	m_pModel = std::make_unique<Model>();
	if (!m_pModel->Load(MODELPASS("ball.obj"))) ERROR_MESSAGE("ball.fbx");

	m_pShadow = std::make_unique<Texture>();
	if (FAILED(m_pShadow->Create(TEXPASS("Shadow.png")))) ERROR_MESSAGE("Shadow.png");

	m_tParam.pos = { ce_fBallPos.x + WORLD_AJUST ,ce_fBallPos.y + WORLD_AJUST, ce_fBallPos.z + WORLD_AJUST };
	m_tParam.size = ce_fBallSize;
	m_tParam.rotate = { 0.0f,0.0f,0.0f };
	m_BallCollision.type = Collision::eBox;
	m_BallCollision.box.center = m_tParam.pos;
	m_BallCollision.box.size = { ce_fBallSize.x / 2.0f,ce_fBallSize.y / 2.0f,ce_fBallSize.z / 2.0f };

	m_LucusCollision.type = Collision::eLine;
	m_LucusCollision.line.start = m_tParam.pos;
	m_LucusCollision.line.end = m_tParam.pos;
}

CBall::~CBall()
{
	m_pPitching.release();
	m_pCursor.release();
	m_pPitching.release();
	m_pBatting.release();
}

void CBall::Update()
{
	m_LucusCollision.line.start = m_tParam.pos;
	switch (m_nPhase)
	{
	case (int)BallPhase::Batting: UpdateBatting(); break;
	case (int)BallPhase::InPlay: UpdateInPlay(); break;
	default:
		break;
	}
	m_BallCollision.type = Collision::eBox;
	m_BallCollision.box.center = m_tParam.pos;
	m_BallCollision.box.size = { ce_fBallSize.x / 2.0f,ce_fBallSize.y / 2.0f,ce_fBallSize.z / 2.0f };
	m_LucusCollision.line.end = m_tParam.pos;
}

void CBall::Draw()
{
#ifdef _IMGUI
	DirectX::XMFLOAT3X3 debug = GetPosSizeRotateDebug("Ball");
#endif // _IMGUI

	Collision::DrawCollision(m_LucusCollision);
	SetModel(m_tParam,m_pModel.get());
	
}

void CBall::SetModel(ModelParam param, Model* model, bool isAnime)
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

void CBall::OnCollision(Collision::Result collision)
{
	// フェンスを越えていたらホームラン
	if (m_tParam.pos.y >= ce_fFenceHeight + WORLD_AJUST)
	{
		CRunning::HomeRun();
		return;
	}

	// 越えていなかったらフェンス反射の計算をする
	// 計算に使用する変数を定義
	DirectX::XMVECTOR vecStart = DirectX::XMLoadFloat3(&m_LucusCollision.line.start);
	DirectX::XMVECTOR vecEnd = DirectX::XMLoadFloat3(&m_LucusCollision.line.end);
	DirectX::XMVECTOR vecDir = DirectX::XMVectorSubtract(vecEnd, vecStart);
	DirectX::XMVECTOR vecPoint[3];
	vecPoint[0] = DirectX::XMLoadFloat3(&collision.other.triangle.point[0]);
	vecPoint[1] = DirectX::XMLoadFloat3(&collision.other.triangle.point[1]);
	vecPoint[2] = DirectX::XMLoadFloat3(&collision.other.triangle.point[2]);

	// 衝突位置の検出
	DirectX::XMVECTOR vecHitPoint = DirectX::XMVectorAdd(vecStart, DirectX::XMVectorScale(vecDir, collision.t));

	// 衝突した三角形の法線を求める
	DirectX::XMVECTOR vecEdge1 = DirectX::XMVectorSubtract(vecPoint[1], vecPoint[0]);
	DirectX::XMVECTOR vecEdge2 = DirectX::XMVectorSubtract(vecPoint[2], vecPoint[0]);
	DirectX::XMVECTOR vecNormal = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vecEdge1, vecEdge2));

	// 反射ベクトルの計算 (修正済み)
	float dotProduct = DirectX::XMVectorGetX(DirectX::XMVector3Dot(vecDir, vecNormal));
	DirectX::XMVECTOR vecReflectDir = DirectX::XMVectorSubtract(
		vecDir,
		DirectX::XMVectorScale(vecNormal, 2.0f * dotProduct)
	);
	vecReflectDir = DirectX::XMVector3Normalize(vecReflectDir);

	// 反射後の位置を補正（スタック防止）
	vecHitPoint = DirectX::XMVectorAdd(vecHitPoint, DirectX::XMVectorScale(vecNormal, 5.0f));

	// 速度の減衰
	float fRestitution = 0.8f;
	float ballVelocity = DirectX::XMVectorGetX(DirectX::XMVector3Length(vecDir));

	DirectX::XMVECTOR vecNewVelocity = DirectX::XMVectorScale(vecReflectDir, fRestitution * ballVelocity);

	// 移動先の更新
	DirectX::XMFLOAT3 curDir;
	DirectX::XMStoreFloat3(&curDir, vecDir);
	DirectX::XMStoreFloat3(&m_LucusCollision.line.start,vecHitPoint);
	DirectX::XMStoreFloat3(&m_tParam.pos,vecHitPoint);
	DirectX::XMStoreFloat3(&m_LucusCollision.line.end, DirectX::XMVectorAdd(vecHitPoint, vecNewVelocity));
	DirectX::XMStoreFloat3(&m_fMove, vecNewVelocity);
}

void CBall::OnFoulZone(Collision::Result collision)
{
	m_bBallFaulZone = true;
	m_fFaulZoneBallPos = m_tParam.pos;
}

Collision::Info CBall::GetCollision()
{
	return m_BallCollision;
}

Collision::Info CBall::GetLineCollision()
{
	return m_LucusCollision;
}

void CBall::SetPitching(CPitching* pitching)
{
	m_pPitching.reset(pitching);
}

void CBall::SetPitchingCursor(CPitchingCursor* cursor)
{
	m_pCursor.reset(cursor);
}

void CBall::SetBatting(CBatting* batting)
{
	m_pBatting.reset(batting);
}

DirectX::XMFLOAT3 CBall::GetPos()
{
	return m_tParam.pos;
}

void CBall::SetPos(DirectX::XMFLOAT3 pos)
{
	m_tParam.pos = pos;
}

void CBall::SetMove(DirectX::XMFLOAT3 direction)
{
	m_fMove = direction;
}

std::unique_ptr<CBall>& CBall::GetInstance()
{
	// インスタンスは一つしか存在しない
	static std::unique_ptr<CBall> instance(new CBall());
	return instance;
}

CBall::BallPhase CBall::GetPhase()
{
	return (BallPhase)m_nPhase;
}

bool CBall::GetIsFry()
{
	return m_bFry;
}

void CBall::UpdateBatting()
{
#ifndef _CAM_DEBUG
	CCamera::SetCameraKind(CAM_BATTER);
#endif // !_CAM_DEBUG
	DirectX::XMFLOAT2 fCursorPos;
	DirectX::XMFLOAT2 fCenterToCursor;  
	DirectX::XMFLOAT2 fZoneSizeHarf;
	DirectX::XMFLOAT2 fCenterToCursorPow;
	DirectX::XMFLOAT2 fPredPos;
	DirectX::XMFLOAT2 fCenterToPred;
	DirectX::XMFLOAT2 fCenterToPredPow;
	static DirectX::XMFLOAT2 fBendValue;
	static float fTime = 0.0f;
	static bool bRelease;

	m_tParam.size = ce_fBallSize;
	if (m_pPitching->GetPitchingPhase() == CPitching::PitchingPhase::Release)
	{
		if (!bRelease)
		{
			// 投球地点の取得
			fCursorPos = m_pCursor->GetPos();
			fCenterToCursor = { fCursorPos.x - ce_fPitchingCursorPos.x,fCursorPos.y - ce_fPitchingCursorPos.y };
			fZoneSizeHarf = { ce_fStrikeZoneSize.x / 2.0f,ce_fStrikeZoneSize.y / 2.0f };
			fCenterToCursorPow = { fCenterToCursor.x / fZoneSizeHarf.x,fCenterToCursor.y / fZoneSizeHarf.y };
			m_fPitchPos = { ce_fBallLimitX.x * fCenterToCursorPow.x,  ce_fBallLimitX.y * fCenterToCursorPow.y };
			m_fPitchPos.x *= -1.0f;
			m_fPitchPos.y *= -1.0f;

			// 予測地点の取得
			fPredPos = m_pCursor->GetPredPos();
			fCenterToPred = { fPredPos.x - fCursorPos.x,fPredPos.y - fCursorPos.y };
			fCenterToPredPow = { fCenterToPred.x / fZoneSizeHarf.x,fCenterToPred.y / fZoneSizeHarf.y };
			m_fPredValue = { ce_fBallLimitX.x * fCenterToPredPow.x,  ce_fBallLimitX.y * fCenterToPredPow.y };
			m_fPredValue.x *= -1.0f;
			m_fPredValue.y *= -1.0f;

			// 予測地点の取得
			m_fPredValue.x *= (ce_fBallEndPos.z - ce_fBallPos.z) / (134.0f - ce_fBallPos.z);
			m_fPredValue.y *= (ce_fBallEndPos.z - ce_fBallPos.z) / (134.0f - ce_fBallPos.z);

			bRelease = true;
		}

		float fChatch = m_pPitching->GetChatchTime() * fFPS;
		fBendValue.x += m_fPredValue.x / fChatch;
		fBendValue.y += m_fPredValue.y / fChatch;

		fTime += 1.0f / 60.0f;

		m_tParam.pos.x = m_fPitchPos.x + fBendValue.x + ce_fBallEndPos.x + WORLD_AJUST;
		m_tParam.pos.y = m_fPitchPos.y + fBendValue.y + ce_fBallEndPos.y + WORLD_AJUST;

		m_tParam.pos.z += (ce_fBallEndPos.z - ce_fBallPos.z) / fChatch;
		m_tParam.rotate.x += DirectX::XMConvertToRadians(ce_nBallRotateSec * 360.0f) / fChatch;

		if (m_pBatting->GetBatting())
		{
			m_nPhase = (int)BallPhase::InPlay;
			m_fMove = m_pBatting->GetDirection();
			m_bFry = true;
			m_bBallFaulZone = false;
		}
	}
	else
	{
		fTime = 0.0f;
		m_tParam.pos = { ce_fBallPos.x + WORLD_AJUST ,ce_fBallPos.y + WORLD_AJUST, ce_fBallPos.z + WORLD_AJUST };

		bRelease = false;
		fBendValue = {};
	}
}

void CBall::UpdateInPlay()
{
	CBallCount* pBallCount = CBallCount::GetInstance().get();




	m_tParam.size = ce_fInplayBallSize;
#ifndef _CAM_DEBUG
	CCamera::SetCameraKind(CAM_INPLAY);
#endif
	m_tParam.pos.x += m_fMove.x;
	m_tParam.pos.y += m_fMove.y;
	m_tParam.pos.z += m_fMove.z;

	OutputDebugStringA(std::to_string(m_tParam.pos.y).c_str());
	OutputDebugStringA("\n");

	m_fMove.x *= 0.99f;
	m_fMove.y *= 0.99f;
	m_fMove.z *= 0.99f;

	m_fMove.y -= MSEC(GRAVITY);
 
	if (m_tParam.pos.y < 0.0f + WORLD_AJUST + ce_fGroundY)
	{
		if(CFielding::GetChatchPattern() == CFielding::ChatchPattern::NotChatch)m_bFry = false;
		m_fMove.x *= 0.95f;
		m_fMove.y *= 0.5f;
		m_fMove.z *= 0.95f;
	
		m_fMove.y = -m_fMove.y;
		if (m_fMove.y < CMETER(5.0f))
		{
			m_fMove.y = 0.0f;
			m_tParam.pos.y = WORLD_AJUST + ce_fGroundY;
		}
		else 
		{
			m_tParam.pos.y -= WORLD_AJUST + ce_fGroundY;
			m_tParam.pos.y = -m_tParam.pos.y;
			m_tParam.pos.y += WORLD_AJUST + ce_fGroundY;
		}
	}

	float speed;
	DirectX::XMVECTOR vMove = DirectX::XMLoadFloat3(&m_fMove);
	DirectX::XMVECTOR vLen = DirectX::XMVector3Length(vMove);
	DirectX::XMStoreFloat(&speed, vLen);
	if (m_bBallFaulZone)
	{
		// ボールが落ちてからファール判定をする
		if (!m_bFry)
		{
			// ファールゾーンに入った時外野にボールがある時は即時ファールにする
			if (m_fFaulZoneBallPos.z <= ce_fInOutBorderZ)
			{
				for (int i = 0; i < (int)CBallCount::InplayElement::Max; i++)
				{
					pBallCount->SetEndInplay((CBallCount::InplayElement)i, true);
				}
			}
			// ファールゾーンに入った時内野にボールがある時はボールを取った時か止まった時に判定する
			else
			{
				if (CFielding::GetChatchPattern() == CFielding::ChatchPattern::Grounder ||
					(speed < CMSEC(3.0f) && CFielding::GetChatchPattern() == CFielding::ChatchPattern::Grounder))
				{
					for (int i = 0; i < (int)CBallCount::InplayElement::Max; i++)
					{
						pBallCount->SetEndInplay((CBallCount::InplayElement)i, true);
					}
				}
			}
		}
	}

	// インプレーの終了
	if (pBallCount->GetEndInplay())
	{
		for (int i = 0; i < (int)CBallCount::InplayElement::Max; i++)
		{
			pBallCount->SetEndInplay((CBallCount::InplayElement)i, false);
		}
		// ファールの時はカウントをリセットしない
		if (m_bBallFaulZone && CFielding::GetChatchPattern() != CFielding::ChatchPattern::Fry) pBallCount->AddStrikeCount(true);
		else pBallCount->ResetCount();
		m_bBallFaulZone = false;
		m_nPhase = (int)BallPhase::Batting;
		return;
	}
}

void CBall::DrawShadow()
{

}
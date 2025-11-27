#include "Ball.h"
#include "ImGuiManager.h"
#include "SceneGame.h"
#include "Sprite.h"
#include "Main.h"
#include "GameManager.h"
#include "Running.h"
#include "ModelRenderer.h"
#include "Field.h"
#include "Oparation.h"

constexpr DirectX::XMFLOAT2 ce_fStrikeZoneSizeXYIn3D = { 1.54f,1.54f };
constexpr  DirectX::XMFLOAT3 ce_fBallSize = { 0.5f,0.5f,0.5f };
constexpr  DirectX::XMFLOAT3 ce_fInplayBallSize = { 2.0f,2.0f,2.0f };
constexpr int ce_nBallRotateSec = 220 / 60;

CBall::CBall()
	: m_bFryBall(true), m_bPitched(false)
	, m_f3Velocity{}, m_fBallTime(0.0f)
{


	
	m_tParam.m_f3Pos = { ce_fBallPos.x + WORLD_AJUST ,ce_fBallPos.y + WORLD_AJUST, ce_fBallPos.z + WORLD_AJUST };
	m_tParam.m_f3Size = ce_fBallSize;
	m_tParam.m_f3Rotate = { 0.0f,0.0f,0.0f };

	m_pBoxCollision = AddComponent<CCollisionBox>();
	m_pBoxCollision->SetTag("Ball");
	m_pBoxCollision->SetInfo(m_tParam.m_f3Pos, m_tParam.m_f3Size);
	
	m_pLucusCollision = AddComponent<CCollisionLine>();
	m_pLucusCollision->SetTag("BallLine");
	m_pLucusCollision->SetInfo(m_f3OldPos, m_tParam.m_f3Pos);
}

CBall::~CBall()
{

}

void CBall::Init()
{
	CModelRenderer* pModel = AddComponent<CModelRenderer>();
	pModel->Load(PATH_MODEL("ball.obj"));
	pModel->LoadVertexShader(PATH_SHADER("VS_Object.cso"));
	pModel->LoadPixelShader(PATH_SHADER("PS_TexColor.cso"));

	
}

void CBall::Update()
{
	m_f3OldPos = m_tParam.m_f3Pos;
	switch (CGameManager::GetInstance()->GetPhase())
	{
	case GamePhase::Batting: UpdateBatting(); break;
	case GamePhase::InPlay: UpdateInPlay(); break;
	default:
		break;
	}
	m_pBoxCollision->SetInfo(m_tParam.m_f3Pos, m_tParam.m_f3Size * 1.5f);
	m_pLucusCollision->SetInfo(m_f3OldPos, m_tParam.m_f3Pos);

	CGameObject::Update();
}

void CBall::OnCollision(CCollisionBase* other, std::string thisTag, Collision::Result result)
{

	std::string otherTag = other->GetTag();
	Collision::Info otherInfo = other->GetInfo();

	if (dynamic_cast<CField*>(other->GetGameObject()) && thisTag == "BallLine")
	{
		m_bFryBall = true;
		// フェンスを越えていたらホームラン
		if (m_tParam.m_f3Pos.y >= ce_fFenceHeight + WORLD_AJUST && otherTag == "HomeRunFence")
		{
			//CRunning::HomeRun();
			return;
		}

		Collision::Info info = m_pLucusCollision->GetInfo();
		// 越えていなかったらフェンス反射の計算をする
		// 計算に使用する変数を定義
		DirectX::XMVECTOR vecStart = DirectX::XMLoadFloat3(&info.line.start);
		DirectX::XMVECTOR vecEnd = DirectX::XMLoadFloat3(&info.line.end);
		DirectX::XMVECTOR vecDir = DirectX::XMVectorSubtract(vecEnd, vecStart);
		DirectX::XMVECTOR vecPoint[3];
		vecPoint[0] = DirectX::XMLoadFloat3(&otherInfo.triangle.point[0]);
		vecPoint[1] = DirectX::XMLoadFloat3(&otherInfo.triangle.point[1]);
		vecPoint[2] = DirectX::XMLoadFloat3(&otherInfo.triangle.point[2]);

		// 衝突位置の検出
		DirectX::XMVECTOR vecHitPoint = DirectX::XMVectorAdd(vecStart, DirectX::XMVectorScale(vecDir, result.t));

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
		DirectX::XMStoreFloat3(&info.line.start, vecHitPoint);
		DirectX::XMStoreFloat3(&m_tParam.m_f3Pos, vecHitPoint);
		DirectX::XMStoreFloat3(&info.line.end, DirectX::XMVectorAdd(vecHitPoint, vecNewVelocity));
		DirectX::XMStoreFloat3(&m_f3Velocity, vecNewVelocity);
	}
}


void CBall::UpdateBatting()
{
	CCamera::GetInstance()->SetCameraKind(CAM_BATTER);
	static float fTime = 0.0f;
	static bool bRelease;

	m_tParam.m_f3Size = ce_fBallSize;
	if (m_bPitched)
	{
		if (!bRelease)
		{
			DirectX::XMFLOAT3 f3Dir = ce_fJustmeetPos - ce_fBallPos;
			DirectX::XMVECTOR vecDir = DirectX::XMLoadFloat3(&f3Dir);
			DirectX::XMVECTOR vecInitPos = DirectX::XMLoadFloat3(&ce_fBallPos);

			CPitchingCursor* pCursor = GetScene()->GetGameObject<CPitchingCursor>();
			DirectX::XMFLOAT3 f3PredPos = pCursor->GetPredPos();
			DirectX::XMFLOAT3 f3OffsetFromCenter = ce_fStrikeZonePos - f3PredPos;
			DirectX::XMFLOAT3 f3BendedPos = DirectX::XMFLOAT3(ce_fJustmeetPos.x + 0.0f, ce_fJustmeetPos.y + 0.0f, ce_fJustmeetPos.z);
			DirectX::XMVECTOR vecZoneCenterPos = DirectX::XMLoadFloat3(&f3BendedPos);
			vecDir = DirectX::XMVector3Normalize(vecDir);
			float Az = DirectX::XMVectorGetZ(vecInitPos);
			float Bz = DirectX::XMVectorGetZ(vecZoneCenterPos);

			float dz = Bz - Az;

			// t = (targetZ - A.z) / (B.z - A.z)
			float t = (-224.0f - Az) / dz;

			// C = A + t(B - A)
			DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(vecZoneCenterPos, vecInitPos);
			DirectX::XMVECTOR targetC = DirectX::XMVectorAdd(vecInitPos, DirectX::XMVectorScale(AB, t));

			// 誤差をなくす
			targetC = DirectX::XMVectorSetZ(targetC, -224.0f);
			DirectX::XMVECTOR vecLengthOfPitch = targetC - vecInitPos;
			DirectX::XMVECTOR vecVel = DirectX::XMVectorScale(vecLengthOfPitch, 1.0f / (fFPS * m_fBallTime));
			DirectX::XMStoreFloat3(&m_f3Velocity, vecVel);
			bRelease = true;
		}


		fTime += 1.0f / fFPS;
		m_tParam.m_f3Pos += m_f3Velocity;

		if (m_fBallTime <= fTime)
		{
			m_fBallTime = 0.0f;
			fTime = 0.0f;
			m_bPitched = false;
		}
	}
	else
	{
		fTime = 0.0f;
		m_tParam.m_f3Pos = { ce_fBallPos.x + WORLD_AJUST ,ce_fBallPos.y + WORLD_AJUST, ce_fBallPos.z + WORLD_AJUST };

		bRelease = false;
	}
}

void CBall::UpdateInPlay()
{
	CGameManager* pBallCount = CGameManager::GetInstance();




	m_tParam.m_f3Size= ce_fInplayBallSize;
	CCamera::GetInstance()->SetCameraKind(CAM_INPLAY);
	

	m_f3Velocity.x *= 0.99f;
	m_f3Velocity.y *= 0.99f;
	m_f3Velocity.z *= 0.99f;

	m_f3Velocity.y -= MSEC(GRAVITY);
 
	if (m_tParam.m_f3Pos.y < 0.0f + WORLD_AJUST + ce_fGroundY)
	{
		m_bFryBall = true;
		//if(CFielding::GetChatchPattern() == CFielding::ChatchPattern::NotChatch)m_bFry = false;
		m_f3Velocity.x *= 0.95f;
		m_f3Velocity.y *= 0.5f;
		m_f3Velocity.z *= 0.95f;
	
		m_f3Velocity.y = -m_f3Velocity.y;
		if (m_f3Velocity.y < CMETER(5.0f))
		{
			m_f3Velocity.y = 0.0f;
			m_tParam.m_f3Pos.y = WORLD_AJUST + ce_fGroundY;
		}
		else 
		{
			m_tParam.m_f3Pos.y -= WORLD_AJUST + ce_fGroundY;
			m_tParam.m_f3Pos.y = -m_tParam.m_f3Pos.y;
			m_tParam.m_f3Pos.y += WORLD_AJUST + ce_fGroundY;
		}
	}

	float speed;
	DirectX::XMVECTOR vMove = DirectX::XMLoadFloat3(&m_f3Velocity);
	DirectX::XMVECTOR vLen = DirectX::XMVector3Length(vMove);
	DirectX::XMStoreFloat(&speed, vLen);
	//if (m_bBallFaulZone)
	{
		// ボールが落ちてからファール判定をする
		if (!m_bFryBall)
		{
			// ファールゾーンに入った時外野にボールがある時は即時ファールにする
			//if (m_fFaulZoneBallPos.z <= ce_fInOutBorderZ)
			//{
			//	for (int i = 0; i < (int)CGameManager::InplayElement::Max; i++)
			//	{
			//		pBallCount->SetEndInplay((CGameManager::InplayElement)i, true);
			//	}
			//}
			//// ファールゾーンに入った時内野にボールがある時はボールを取った時か止まった時に判定する
			//else
			//{
			//	if (CFielding::GetChatchPattern() == CFielding::ChatchPattern::Grounder ||
			//		(speed < CMSEC(3.0f) && CFielding::GetChatchPattern() == CFielding::ChatchPattern::Grounder))
			//	{
			//		for (int i = 0; i < (int)CGameManager::InplayElement::Max; i++)
			//		{
			//			pBallCount->SetEndInplay((CGameManager::InplayElement)i, true);
			//		}
			//	}
			//}
		}
	}

	// インプレーの終了
	//if (pBallCount->GetEndInplay())
	//{
	//	for (int i = 0; i < (int)CGameManager::InplayElement::Max; i++)
	//	{
	//		pBallCount->SetEndInplay((CGameManager::InplayElement)i, false);
	//	}
	//	// ファールの時はカウントをリセットしない
	//	if (m_bBallFaulZone && CFielding::GetChatchPattern() != CFielding::ChatchPattern::Fry) pBallCount->AddStrikeCount(true);
	//	else pBallCount->ResetCount();
	//	m_bBallFaulZone = false;
	//	m_nPhase = (int)BallPhase::Batting;
	//	return;
	//}
}
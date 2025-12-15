#include "Ball.h"
#include "SceneGame.h"
#include "Sprite.h"
#include "Main.h"
#include "GameManager.h"
#include "Running.h"
#include "ModelRenderer.h"
#include "Field.h"
#include "Oparation.h"

constexpr DirectX::XMFLOAT3 ce_fStrikeZoneSizeIn3D = { 1.54f,1.54f,0.0f };
constexpr  DirectX::XMFLOAT3 ce_fBallSize = { 0.5f,0.5f,0.5f };
constexpr  DirectX::XMFLOAT3 ce_fInplayBallSize = { 2.0f,2.0f,2.0f };
constexpr int ce_nBallRotateSec = 220 / 60;

CBall::CBall()
	: CGameObject()
	, m_bFryBall(true), m_bPitched(false), m_bCaught(false), m_bFryChatch(false), m_bInOutField(false)
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
	CModelRenderer* pRenderer = AddComponent<CModelRenderer>();
	pRenderer->Load(PATH_MODEL("ball.obj"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Object.cso"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_TexColor.cso"));
	pRenderer->LoadTexture(PATH_MODEL("ball.png"));

	
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
		// フェンスを越えていたらホームラン
		if (otherTag == "HomeRunFence")
		{
			if (m_tParam.m_f3Pos.y >= ce_fFenceHeight + WORLD_AJUST)
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
			return;
		}

		if (otherTag == "OutField")
		{
			m_bInOutField = true;
			FaulCalc();
			return;
		}
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
			vecDir = DirectX::XMVector3Normalize(vecDir);	// 方向を正規化
			DirectX::XMVECTOR vecInitPos = DirectX::XMLoadFloat3(&ce_fBallPos);

			// スクリーン上の投球予測地点座標から3D座標に投影する
			DirectX::XMFLOAT3 f3Up = CCamera::GetInstance()->GetUp();
			DirectX::XMVECTOR vecRight = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&f3Up), vecDir));
			DirectX::XMFLOAT3 f3Right;
			DirectX::XMStoreFloat3(&f3Right, vecRight);
			CPitchingCursor* pCursor = GetScene()->GetGameObject<CPitchingCursor>();
			DirectX::XMFLOAT3 f3PredPos = pCursor->GetPredPos();	// スクリーン上の投球予測点	
			DirectX::XMFLOAT3 f3OffsetFromCenter = ce_fStrikeZonePos - f3PredPos;	// ストライクゾーン中央からの距離を求める
			DirectX::XMFLOAT3 f3StrikeZoneHarfSize = ce_fStrikeZoneSize / 2.0f;		// ストライクゾーンの半分のサイズを求める
			DirectX::XMFLOAT3 f3ZoneLimitToCursor = f3OffsetFromCenter / f3StrikeZoneHarfSize;	// ゾーンハーフサイズの地点を+-1.0fとして、カーソル位置を-1~1に正規化する
			DirectX::XMFLOAT3 f3CusorPos3D = f3ZoneLimitToCursor * ce_fStrikeZoneSizeIn3D;	// 正規化したカーソル位置を3D空間上のゾーンサイズを使って3D空間に投影する
			DirectX::XMFLOAT3 f3BendedPos = DirectX::XMFLOAT3(ce_fJustmeetPos.x + f3CusorPos3D.x, ce_fJustmeetPos.y + f3CusorPos3D.y, ce_fJustmeetPos.z);	// 求めたカーソル位置は中心からのOffSet座標なので、ゾーンの中心座標を加算する
			f3BendedPos.x *= f3Right.x;

			// 投球予測地点への方向でキャッチャーミットまでの距離を移動するVelocityを求める
			DirectX::XMVECTOR vecZoneCenterPos = DirectX::XMLoadFloat3(&f3BendedPos);	// 投球予測地点
			float Az = DirectX::XMVectorGetZ(vecInitPos);	// 投球位置(投手の位置)のZ位置
			float Bz = DirectX::XMVectorGetZ(vecZoneCenterPos);	// ストライクゾーン上のZ位置

			float dz = Bz - Az;	// 直線状の距離を求める

			// t = (targetZ - A.z) / (B.z - A.z)として、ABベクトルを通るtargetZ位置ベクトルの長さを求める
			float t = (-224.0f - Az) / dz;

			// C = A + t(B - A)として、targetCの座標ベクトルを求める
			DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(vecZoneCenterPos, vecInitPos);
			DirectX::XMVECTOR targetC = DirectX::XMVectorAdd(vecInitPos, DirectX::XMVectorScale(AB, t));

			// 誤差をなくす
			targetC = DirectX::XMVectorSetZ(targetC, -224.0f);

			// 投球位置からキャッチャー位置までの距離を求める
			DirectX::XMVECTOR vecLengthOfPitch = targetC - vecInitPos;
			// その距離をm_fBallTime秒で進み終わるようにVelocityを求める
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

void CBall::SetCaught(bool isChatch)
{
	if (isChatch && !m_bFryBall)  FaulCalc();
	m_bCaught = isChatch;
}

void CBall::CheckFaul()
{
	// 守備側が捕球してない時
	if (!m_bCaught)
	{
		DirectX::XMVECTOR vecVelScale = DirectX::XMVector3Length(DirectX::XMLoadFloat3(&m_f3Velocity));
		float fVelPower = 0.0f;
		DirectX::XMStoreFloat(&fVelPower, vecVelScale);
		// フライならファール判定を行わない
		if (m_bFryBall) return;
		// 内野でボールが動いている時もフェアに戻るかもしれないのでファール判定を行わない
		else if (!m_bInOutField && fVelPower > 1.0f) return;
		// 外野でのファール判定は基本行わない(外野に行った一度限りのタイミングで行う)
		else if (m_bInOutField) return;
	}
	// 捕球した時
	else
	{
		// フライキャッチはアウトなのでファール判定を行わない
		// ゴロキャッチ時のファール判定はキャッチしたタイミングで行う
		return;
	}

	FaulCalc();
}

void CBall::FaulCalc()
{
	CScene* pScene = GetScene();
	CField* pField = pScene->GetGameObject<CField>();
	std::array<DirectX::XMFLOAT3, 2> f3BaseLinePoints = pField->GetFirstBaseLinePoint();
	DirectX::XMVECTOR vecBallPoint = DirectX::XMLoadFloat3(&m_tParam.m_f3Pos);

	DirectX::XMVECTOR vecLineStart = DirectX::XMLoadFloat3(&f3BaseLinePoints[0]);
	DirectX::XMVECTOR vecLineEnd = DirectX::XMLoadFloat3(&f3BaseLinePoints[1]);

	// 線分ベクトルと点方向ベクトル
	DirectX::XMVECTOR vecFirstLine = DirectX::XMVectorSubtract(vecLineEnd, vecLineStart);
	DirectX::XMVECTOR vecStartToPoint = DirectX::XMVectorSubtract(vecBallPoint, vecLineStart);

	// 基準となる法線（例：上方向 Y+）
	DirectX::XMVECTOR N = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// クロス積
	DirectX::XMVECTOR cross = DirectX::XMVector3Cross(vecFirstLine, vecStartToPoint);

	// 左右判定（符号で決まる）
	float sideFirst = DirectX::XMVectorGetX(DirectX::XMVector3Dot(cross, N));

	f3BaseLinePoints = pField->GetThirdBaseLinePoint();

	vecLineStart = DirectX::XMLoadFloat3(&f3BaseLinePoints[0]);
	vecLineEnd = DirectX::XMLoadFloat3(&f3BaseLinePoints[1]);

	// 線分ベクトルと点方向ベクトル
	vecFirstLine = DirectX::XMVectorSubtract(vecLineEnd, vecLineStart);
	vecStartToPoint = DirectX::XMVectorSubtract(vecBallPoint, vecLineStart);

	// クロス積
	cross = DirectX::XMVector3Cross(vecFirstLine, vecStartToPoint);

	// 左右判定（符号で決まる）
	float sideThird = DirectX::XMVectorGetX(DirectX::XMVector3Dot(cross, N));

	if (sideFirst < 0 || sideThird > 0)
	{
		CGameManager::GetInstance()->FaulBall();
	}
}

void CBall::UpdateInPlay()
{
	m_tParam.m_f3Size= ce_fInplayBallSize;
	CCamera::GetInstance()->SetCameraKind(CAM_INPLAY);
	

	m_f3Velocity.x *= 0.99f;
	m_f3Velocity.y *= 0.99f;
	m_f3Velocity.z *= 0.99f;


	m_tParam.m_f3Pos += m_f3Velocity;


	m_f3Velocity.y -= MSEC(GRAVITY);
 
	if (m_tParam.m_f3Pos.y < 0.0f + WORLD_AJUST + ce_fGroundY)
	{
		if (m_bFryBall && m_bInOutField) FaulCalc();
		m_bFryBall = false;
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
	CheckFaul();
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
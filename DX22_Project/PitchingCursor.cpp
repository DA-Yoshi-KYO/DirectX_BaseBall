// ==============================
//    インクルード部
// ==============================
#include "PitchingCursor.h"
#include "Sprite.h"
#include "Main.h"
#include "Camera.h"
#include "Input.h"
#include "SceneGame.h"
#include "ImGuiManager.h"
#include "BallCount.h"
#include "Pitching.h"
#include "Controller.h"
#include "SpriteRenderer.h"
#include "GameManager.h"
#include "StrikeZone.h"

// ==============================
//    定数定義
// ==============================
// 基礎変化量
constexpr DirectX::XMFLOAT3 ce_fBenderValue[(int)CPitching::BenderKind::Max] =
{
	{ 0.0f,0.0f,0.0f },		// ストレートの基礎変化量
	{ 5.0f,5.0f,0.0f },		// ツーシームの基礎変化量
	{ 10.0f,0.0f,0.0f },		// スライダーの基礎変化量
	{ 10.0f,10.0f,0.0f },	// カーブの基礎変化量
	{ 0.0f,10.0f,0.0f },		// フォークの基礎変化量
	{ -10.0f,10.0f,0.0f },	// シンカーの基礎変化量
	{ -10.0f,0.0f,0.0f },	// シュートの基礎変化量
};

CPitchingCursor::CPitchingCursor()
	: m_bMove{ true }
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("Ball.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite"));
	// テクスチャパラメータの初期化
	m_tBallParam.m_f3Pos = ce_fPitchingCursorPos;
	m_tBallParam.m_f3Size = DirectX::XMFLOAT3(20.0f, 20.0f,0.0f);
	m_tBallParam.m_f3Rotate = DirectX::XMFLOAT3();
	m_tBallParam.m_f4Color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tBallParam.m_f2UVPos = DirectX::XMFLOAT2(0.0f, 0.0f);
	m_tBallParam.m_f2UVSize = DirectX::XMFLOAT2(1.0f, 1.0f);

	m_tPredParam.m_f3Pos = ce_fPitchingCursorPos;
	m_tPredParam.m_f3Size = DirectX::XMFLOAT3(20.0f, 20.0f,0.0f);
	m_tPredParam.m_f3Rotate = DirectX::XMFLOAT3();;
	m_tPredParam.m_f4Color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f);
	m_tPredParam.m_f2UVPos = DirectX::XMFLOAT2(0.0f, 0.0f);
	m_tPredParam.m_f2UVSize = DirectX::XMFLOAT2(1.0f, 1.0f);



	// コリジョン情報の初期化
	m_Collision.type = Collision::Type2D::eCircle;
	m_Collision.square.pos = { m_tBallParam.m_f3Pos.x, m_tBallParam.m_f3Pos.y };
	m_Collision.square.size = { m_tBallParam.m_f3Size.x,m_tBallParam.m_f3Size.y };
	m_Collision.circle.pos = {m_tBallParam.m_f3Pos.x, m_tBallParam.m_f3Pos.y
};
	m_Collision.circle.radius = m_tBallParam.m_f3Size.x;
}

CPitchingCursor::~CPitchingCursor()
{

}

void CPitchingCursor::Update()
{
	CPitching* pPitching = CGameManager::GetInstance()->GetDefenceManager()->GetPitching();
	CPitching::PitchState tState = pPitching->GetPitchState();
	m_tPredParam.m_f3Pos = m_tBallParam.m_f3Pos;
	CStrikeZone* pStrikeZone = GetScene()->GetGameObject<CStrikeZone>();
	DirectX::XMFLOAT3 fStrikeZonePos = pStrikeZone->GetPos();
	DirectX::XMFLOAT3 fStrikeZoneSize = pStrikeZone->GetSize();
	// カーソル移動可能なときに移動処理をする
	if (m_bMove && pPitching->GetPitchingPhase() == CPitching::PitchingPhase::Pitch)
	{
		// 移動処理
		DirectX::XMFLOAT2 fInput = {};
		DirectX::XMFLOAT3 fMaxPos = { fabsf(fStrikeZonePos.x) + fStrikeZoneSize.x / 1.5f , fabsf(fStrikeZonePos.y) + fStrikeZoneSize.y / 1.5f,0.0f };
		m_tBallParam.m_f3Pos = { fStrikeZonePos.x + fInput.x * fMaxPos.x,fStrikeZonePos.y + fInput.y * fMaxPos.y,0.0f };

		// 移動補正
		if (m_tBallParam.m_f3Pos.x >= fStrikeZonePos.x + fStrikeZoneSize.x / 1.3f) m_tBallParam.m_f3Pos.x = fStrikeZonePos.x + fStrikeZoneSize.x / 1.3f;
		if (m_tBallParam.m_f3Pos.x <= fStrikeZonePos.x - fStrikeZoneSize.x / 1.3f) m_tBallParam.m_f3Pos.x = fStrikeZonePos.x - fStrikeZoneSize.x / 1.3f;
		if (m_tBallParam.m_f3Pos.y >= fStrikeZonePos.y + fStrikeZoneSize.y / 1.3f) m_tBallParam.m_f3Pos.y = fStrikeZonePos.y + fStrikeZoneSize.y / 1.3f;
		if (m_tBallParam.m_f3Pos.y <= fStrikeZonePos.y - fStrikeZoneSize.y / 1.3f) m_tBallParam.m_f3Pos.y = fStrikeZonePos.y - fStrikeZoneSize.y / 1.3f;
	}
	else if(pPitching->GetPitchingPhase() == CPitching::PitchingPhase::Release)
	{
		m_tBallParam.m_f3Pos = m_tPredParam.m_f3Pos;
	}
	else if(pPitching->GetPitchingPhase() == CPitching::PitchingPhase::Set)
	{
		m_tBallParam.m_f3Pos = fStrikeZonePos;
	}


	// 予測地点
	// 球種と変化量に応じて投球予測地点を決める
	switch (tState.m_eThrowKind)
	{
	case CPitching::BenderKind::Fourseam:
		m_tPredParam.m_f3Pos = m_tBallParam.m_f3Pos;
		break;
	case CPitching::BenderKind::Twoseam:
		if (tState.m_nBenderQuality[(int)CPitching::BenderKind::Twoseam] != 0)
		{
			if(tState.m_bLeftPitcher) m_tPredParam.m_f3Pos = { m_tBallParam.m_f3Pos.x + ce_fBenderValue[(int)CPitching::BenderKind::Twoseam].x ,m_tBallParam.m_f3Pos.y - ce_fBenderValue[(int)CPitching::BenderKind::Twoseam].y, 0.0f };
			else m_tPredParam.m_f3Pos = { m_tBallParam.m_f3Pos.x - ce_fBenderValue[(int)CPitching::BenderKind::Twoseam].x ,m_tBallParam.m_f3Pos.y - ce_fBenderValue[(int)CPitching::BenderKind::Twoseam].y, 0.0f };
		}
		break;
	case CPitching::BenderKind::Slider:
		if (tState.m_nBenderQuality[(int)CPitching::BenderKind::Slider] != 0)
		{
			if (tState.m_bLeftPitcher)
			{
				m_tPredParam.m_f3Pos = { m_tBallParam.m_f3Pos.x - ce_fBenderValue[(int)CPitching::BenderKind::Slider].x * tState.m_nBenderQuality[(int)CPitching::BenderKind::Slider],
				   m_tBallParam.m_f3Pos.y - ce_fBenderValue[(int)CPitching::BenderKind::Slider].y
				, 0.0f};
			}
			else
			{
				m_tPredParam.m_f3Pos = { m_tBallParam.m_f3Pos.x + ce_fBenderValue[(int)CPitching::BenderKind::Slider].x * tState.m_nBenderQuality[(int)CPitching::BenderKind::Slider] ,
					m_tBallParam.m_f3Pos.y - ce_fBenderValue[(int)CPitching::BenderKind::Slider].y * tState.m_nBenderQuality[(int)CPitching::BenderKind::Slider] 
				, 0.0f };
			}
		}
		break;
	case CPitching::BenderKind::Curve:
		if (tState.m_nBenderQuality[(int)CPitching::BenderKind::Curve] != 0)
		{
			if (tState.m_bLeftPitcher) 
			{
				m_tPredParam.m_f3Pos = { m_tBallParam.m_f3Pos.x - ce_fBenderValue[(int)CPitching::BenderKind::Curve].x * tState.m_nBenderQuality[(int)CPitching::BenderKind::Curve],
					m_tBallParam.m_f3Pos.y - ce_fBenderValue[(int)CPitching::BenderKind::Curve].y * tState.m_nBenderQuality[(int)CPitching::BenderKind::Curve], 0.0f };
			}
			else
			{
				m_tPredParam.m_f3Pos = { m_tBallParam.m_f3Pos.x + ce_fBenderValue[(int)CPitching::BenderKind::Curve].x * tState.m_nBenderQuality[(int)CPitching::BenderKind::Curve]
					,m_tBallParam.m_f3Pos.y - ce_fBenderValue[(int)CPitching::BenderKind::Curve].y * tState.m_nBenderQuality[(int)CPitching::BenderKind::Curve], 0.0f };
			}
		}
		break;
	case CPitching::BenderKind::Split:
		m_tPredParam.m_f3Pos = { m_tBallParam.m_f3Pos.x + ce_fBenderValue[(int)CPitching::BenderKind::Split].x * tState.m_nBenderQuality[(int)CPitching::BenderKind::Split],
			m_tBallParam.m_f3Pos.y - ce_fBenderValue[(int)CPitching::BenderKind::Split].y * tState.m_nBenderQuality[(int)CPitching::BenderKind::Split], 0.0f };
		
		break;
	case CPitching::BenderKind::Sinker:
		if (tState.m_nBenderQuality[(int)CPitching::BenderKind::Sinker] != 0)
		{
			if (tState.m_bLeftPitcher)
			{
				m_tPredParam.m_f3Pos = { m_tBallParam.m_f3Pos.x - ce_fBenderValue[(int)CPitching::BenderKind::Sinker].x * tState.m_nBenderQuality[(int)CPitching::BenderKind::Sinker] ,
					m_tBallParam.m_f3Pos.y - ce_fBenderValue[(int)CPitching::BenderKind::Sinker].y * tState.m_nBenderQuality[(int)CPitching::BenderKind::Sinker], 0.0f };
			}
			else
			{
				m_tPredParam.m_f3Pos = { m_tBallParam.m_f3Pos.x + ce_fBenderValue[(int)CPitching::BenderKind::Sinker].x * tState.m_nBenderQuality[(int)CPitching::BenderKind::Sinker],
					m_tBallParam.m_f3Pos.y - ce_fBenderValue[(int)CPitching::BenderKind::Sinker].y * tState.m_nBenderQuality[(int)CPitching::BenderKind::Sinker] , 0.0f };
			}
		}
		break;
	case CPitching::BenderKind::Shoot:
		if (tState.m_nBenderQuality[(int)CPitching::BenderKind::Shoot] != 0)
		{
			if (tState.m_bLeftPitcher)
			{
				m_tPredParam.m_f3Pos = { m_tBallParam.m_f3Pos.x - ce_fBenderValue[(int)CPitching::BenderKind::Shoot].x * tState.m_nBenderQuality[(int)CPitching::BenderKind::Shoot] ,
					m_tBallParam.m_f3Pos.y - ce_fBenderValue[(int)CPitching::BenderKind::Shoot].y * tState.m_nBenderQuality[(int)CPitching::BenderKind::Shoot], 0.0f };
			}
			else
			{
				m_tPredParam.m_f3Pos = { m_tBallParam.m_f3Pos.x + ce_fBenderValue[(int)CPitching::BenderKind::Shoot].x * tState.m_nBenderQuality[(int)CPitching::BenderKind::Shoot] ,
					m_tBallParam.m_f3Pos.y - ce_fBenderValue[(int)CPitching::BenderKind::Shoot].y * tState.m_nBenderQuality[(int)CPitching::BenderKind::Shoot], 0.0f };
			}
		}
		break;
	default:
		break;
	}

	// コリジョン情報更新
	m_Collision.square.pos = { m_tPredParam.m_f3Pos.x,m_tPredParam.m_f3Pos.y };
	m_Collision.square.size = { m_tPredParam.m_f3Size.x,m_tPredParam.m_f3Size.y };
	m_Collision.circle.pos = { m_tPredParam.m_f3Pos.x, m_tPredParam.m_f3Pos.y };
	m_Collision.circle.radius = m_tPredParam.m_f3Size.x / 2.0f;
}

void CPitchingCursor::Draw()
{
	m_tParam = m_tBallParam;
	CGameObject::Draw();

	m_tParam = m_tPredParam;
	CGameObject::Draw();
}

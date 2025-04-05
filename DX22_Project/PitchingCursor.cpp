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

// ==============================
//    定数定義
// ==============================
// 基礎変化量
constexpr DirectX::XMFLOAT2 ce_fBenderValue[(int)CPitching::BenderKind::Max] =
{
	{ 0.0f,0.0f },		// ストレートの基礎変化量
	{ 5.0f,5.0f },		// ツーシームの基礎変化量
	{ 10.0f,0.0f },		// スライダーの基礎変化量
	{ 10.0f,10.0f },	// カーブの基礎変化量
	{ 0.0f,10.0f },		// フォークの基礎変化量
	{ -10.0f,10.0f },	// シンカーの基礎変化量
	{ -10.0f,0.0f },	// シュートの基礎変化量
};

// ==============================
//    静的変数の初期化
// ==============================
Collision::Info2D CPitchingCursor::m_Collision = {};

CPitchingCursor::CPitchingCursor()
	: m_pTexture{ nullptr }, m_pStrikeZone(nullptr)
	, m_bMove{ true }
{
	// テクスチャの読み込み
	m_pTexture = std::make_unique<Texture>();
	if (FAILED(m_pTexture->Create(TEXPASS("Ball.png")))) MessageBox(NULL, "Ball.png", "Error", MB_OK);

	// テクスチャパラメータの初期化
	m_tParam.pos = ce_fPitchingCursorPos;
	m_tParam.size = DirectX::XMFLOAT2(20.0f, 20.0f);
	m_tParam.rotate = 0.0f;
	m_tParam.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tParam.uvPos = DirectX::XMFLOAT2(0.0f, 0.0f);
	m_tParam.uvSize = DirectX::XMFLOAT2(1.0f, 1.0f);
	m_tParam.world = CCamera::Get2DWolrdMatrix();
	m_tParam.view = CCamera::Get2DViewMatrix();
	m_tParam.proj = CCamera::Get2DProjectionMatrix();
	
	m_tPredParam.pos = ce_fPitchingCursorPos;
	m_tPredParam.size = DirectX::XMFLOAT2(20.0f, 20.0f);
	m_tPredParam.rotate = 0.0f;
	m_tPredParam.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f);
	m_tPredParam.uvPos = DirectX::XMFLOAT2(0.0f, 0.0f);
	m_tPredParam.uvSize = DirectX::XMFLOAT2(1.0f, 1.0f);
	m_tPredParam.world = CCamera::Get2DWolrdMatrix();
	m_tPredParam.view = CCamera::Get2DViewMatrix();
	m_tPredParam.proj = CCamera::Get2DProjectionMatrix();

	
	// コリジョン情報の初期化
	m_Collision.type = Collision::Type2D::eCircle;
	m_Collision.square.pos = m_tParam.pos;
	m_Collision.square.size = m_tParam.size;
	m_Collision.circle.pos = m_tParam.pos;
	m_Collision.circle.radius = m_tParam.size.x;
}

CPitchingCursor::~CPitchingCursor()
{
	// コンポジションインスタンスの放棄
	m_pStrikeZone.release();
	m_pPitching.release();
}

void CPitchingCursor::Update()
{
	CPitching::PitchState tState = m_pPitching->GetPitchState();
	m_tPredParam.pos = m_tParam.pos;

	// カーソル移動可能なときに移動処理をする
	if (m_bMove && m_pPitching->GetPitchingPhase() == CPitching::PitchingPhase::Pitch)
	{
		CBallCount* pBallCount = CBallCount::GetInstance().get();
		DirectX::XMFLOAT2 fStrikeZonePos = m_pStrikeZone->GetPos();
		DirectX::XMFLOAT2 fStrikeZoneSize = m_pStrikeZone->GetSize();

		// 移動処理
		DirectX::XMFLOAT2 fInput = pBallCount->GetDefenseTeam() == CBallCount::Team::Player1 ? CGetLStick((int)CBallCount::Team::Player1) : CGetLStick((int)CBallCount::Team::Player2);
		DirectX::XMFLOAT2 fMaxPos = { fabsf(fStrikeZonePos.x) + fStrikeZoneSize.x / 1.5f , fabsf(fStrikeZonePos.y) + fStrikeZoneSize.y / 1.5f };
		m_tParam.pos = { fStrikeZonePos.x + fInput.x * fMaxPos.x,fStrikeZonePos.y + fInput.y * fMaxPos.y };


		// 移動補正
		if (m_tParam.pos.x >= fStrikeZonePos.x + fStrikeZoneSize.x / 1.3f) m_tParam.pos.x = fStrikeZonePos.x + fStrikeZoneSize.x / 1.3f;
		if (m_tParam.pos.x <= fStrikeZonePos.x - fStrikeZoneSize.x / 1.3f) m_tParam.pos.x = fStrikeZonePos.x - fStrikeZoneSize.x / 1.3f;
		if (m_tParam.pos.y >= fStrikeZonePos.y + fStrikeZoneSize.y / 1.3f) m_tParam.pos.y = fStrikeZonePos.y + fStrikeZoneSize.y / 1.3f;
		if (m_tParam.pos.y <= fStrikeZonePos.y - fStrikeZoneSize.y / 1.3f) m_tParam.pos.y = fStrikeZonePos.y - fStrikeZoneSize.y / 1.3f;
	}

	// 予測地点
	// 球種と変化量に応じて投球予測地点を決める
	switch (tState.m_eThrowKind)
	{
	case CPitching::BenderKind::Fourseam:
		m_tPredParam.pos = m_tParam.pos;
		break;
	case CPitching::BenderKind::Twoseam:
		if (tState.m_nBenderQuality[(int)CPitching::BenderKind::Twoseam] != 0)
		{
			if(tState.m_bLeftPitcher) m_tPredParam.pos = { m_tParam.pos.x + ce_fBenderValue[(int)CPitching::BenderKind::Twoseam].x ,m_tParam.pos.y - ce_fBenderValue[(int)CPitching::BenderKind::Twoseam].y};
			else m_tPredParam.pos = { m_tParam.pos.x - ce_fBenderValue[(int)CPitching::BenderKind::Twoseam].x ,m_tParam.pos.y - ce_fBenderValue[(int)CPitching::BenderKind::Twoseam].y };
		}
		break;
	case CPitching::BenderKind::Slider:
		if (tState.m_nBenderQuality[(int)CPitching::BenderKind::Slider] != 0)
		{
			if (tState.m_bLeftPitcher)
			{
				m_tPredParam.pos = { m_tParam.pos.x - ce_fBenderValue[(int)CPitching::BenderKind::Slider].x * tState.m_nBenderQuality[(int)CPitching::BenderKind::Slider],
				   m_tParam.pos.y - ce_fBenderValue[(int)CPitching::BenderKind::Slider].y };
			}
			else
			{
				m_tPredParam.pos = { m_tParam.pos.x + ce_fBenderValue[(int)CPitching::BenderKind::Slider].x * tState.m_nBenderQuality[(int)CPitching::BenderKind::Slider] ,
					m_tParam.pos.y - ce_fBenderValue[(int)CPitching::BenderKind::Slider].y * tState.m_nBenderQuality[(int)CPitching::BenderKind::Slider] };
			}
		}
		break;
	case CPitching::BenderKind::Curve:
		if (tState.m_nBenderQuality[(int)CPitching::BenderKind::Curve] != 0)
		{
			if (tState.m_bLeftPitcher) 
			{
				m_tPredParam.pos = { m_tParam.pos.x - ce_fBenderValue[(int)CPitching::BenderKind::Curve].x * tState.m_nBenderQuality[(int)CPitching::BenderKind::Curve],
					m_tParam.pos.y - ce_fBenderValue[(int)CPitching::BenderKind::Curve].y * tState.m_nBenderQuality[(int)CPitching::BenderKind::Curve] };
			}
			else
			{
				m_tPredParam.pos = { m_tParam.pos.x + ce_fBenderValue[(int)CPitching::BenderKind::Curve].x * tState.m_nBenderQuality[(int)CPitching::BenderKind::Curve]
					,m_tParam.pos.y - ce_fBenderValue[(int)CPitching::BenderKind::Curve].y * tState.m_nBenderQuality[(int)CPitching::BenderKind::Curve]};
			}
		}
		break;
	case CPitching::BenderKind::Split:
		m_tPredParam.pos = { m_tParam.pos.x + ce_fBenderValue[(int)CPitching::BenderKind::Split].x * tState.m_nBenderQuality[(int)CPitching::BenderKind::Split],
			m_tParam.pos.y - ce_fBenderValue[(int)CPitching::BenderKind::Split].y * tState.m_nBenderQuality[(int)CPitching::BenderKind::Split] };
		
		break;
	case CPitching::BenderKind::Sinker:
		if (tState.m_nBenderQuality[(int)CPitching::BenderKind::Sinker] != 0)
		{
			if (tState.m_bLeftPitcher)
			{
				m_tPredParam.pos = { m_tParam.pos.x - ce_fBenderValue[(int)CPitching::BenderKind::Sinker].x * tState.m_nBenderQuality[(int)CPitching::BenderKind::Sinker] ,
					m_tParam.pos.y - ce_fBenderValue[(int)CPitching::BenderKind::Sinker].y * tState.m_nBenderQuality[(int)CPitching::BenderKind::Sinker] };
			}
			else
			{
				m_tPredParam.pos = { m_tParam.pos.x + ce_fBenderValue[(int)CPitching::BenderKind::Sinker].x * tState.m_nBenderQuality[(int)CPitching::BenderKind::Sinker],
					m_tParam.pos.y - ce_fBenderValue[(int)CPitching::BenderKind::Sinker].y * tState.m_nBenderQuality[(int)CPitching::BenderKind::Sinker] };
			}
		}
		break;
	case CPitching::BenderKind::Shoot:
		if (tState.m_nBenderQuality[(int)CPitching::BenderKind::Shoot] != 0)
		{
			if (tState.m_bLeftPitcher)
			{
				m_tPredParam.pos = { m_tParam.pos.x - ce_fBenderValue[(int)CPitching::BenderKind::Shoot].x * tState.m_nBenderQuality[(int)CPitching::BenderKind::Shoot] ,
					m_tParam.pos.y - ce_fBenderValue[(int)CPitching::BenderKind::Shoot].y * tState.m_nBenderQuality[(int)CPitching::BenderKind::Shoot] };
			}
			else
			{
				m_tPredParam.pos = { m_tParam.pos.x + ce_fBenderValue[(int)CPitching::BenderKind::Shoot].x * tState.m_nBenderQuality[(int)CPitching::BenderKind::Shoot] ,
					m_tParam.pos.y - ce_fBenderValue[(int)CPitching::BenderKind::Shoot].y * tState.m_nBenderQuality[(int)CPitching::BenderKind::Shoot] };
			}
		}
		break;
	default:
		break;
	}

	// コリジョン情報更新
	m_Collision.square.pos =	m_tPredParam.pos;
	m_Collision.square.size =	m_tPredParam.size;
	m_Collision.circle.pos =	m_tPredParam.pos;
	m_Collision.circle.radius = m_tPredParam.size.x / 2.0f;
}

void CPitchingCursor::Draw()
{
#ifdef _COLLISION_DEBUG

	static bool bDebug[2] = { false };

	ApperCollisionSquare(m_Collision.square.pos, m_Collision.square.size, "PitchingSquare", &bDebug[0], { 420.0f,20.0f }, {0.0f,0.0f,255.0f,50.0f});
	ApperCollisionCircle(m_Collision.circle.pos, m_Collision.circle.radius, "PitchingCircle", &bDebug[1], { 620.0f,20.0f }, { 0.0f,0.0f,255.0f,50.0f });

#endif // _COLLISION_DEBUG

	SetRender2D();
	Sprite::SetParam(m_tParam);
	Sprite::SetTexture(m_pTexture.get());
	Sprite::Draw();

	Sprite::SetParam(m_tPredParam);
	Sprite::SetTexture(m_pTexture.get());
	Sprite::Draw();
}

void CPitchingCursor::SetStrikeZone(CStrikeZone* zone)
{
	m_pStrikeZone.reset(zone);
}

void CPitchingCursor::SetPitching(CPitching* pitching)
{
	m_pPitching.reset(pitching);
}

DirectX::XMFLOAT2 CPitchingCursor::GetPos()
{
	return m_tParam.pos;
}

DirectX::XMFLOAT2 CPitchingCursor::GetPredPos()
{
	return m_tPredParam.pos;
}

DirectX::XMFLOAT2 CPitchingCursor::GetSize()
{
	return m_tParam.size;
}

void CPitchingCursor::SetPos(DirectX::XMFLOAT2 pos)
{
	m_tParam.pos = pos;
}

void CPitchingCursor::SetPredPos(DirectX::XMFLOAT2 pos)
{
	m_tPredParam.pos = pos;
}

void CPitchingCursor::SetMove(bool isMove)
{
	m_bMove = isMove;
}

Collision::Info2D CPitchingCursor::GetCollision(bool isCursorOnry, Collision::Type2D type)
{
	m_Collision.type = type;
	Collision::Info2D out = m_Collision;

	if (isCursorOnry)
	{
		out.square.size = { 0.0f,0.0f };
		out.circle.radius = 0.0f;
	}

	return out;
}

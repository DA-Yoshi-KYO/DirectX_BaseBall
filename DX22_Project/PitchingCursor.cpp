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
}

void CPitchingCursor::Update()
{
	CBallCount* pBallCount = CBallCount::GetInstance().get();

	// カーソル移動可能なときに移動処理をする
	if (m_bMove)
	{
		// 移動処理
		if (pBallCount->GetDefenseTeam() == CBallCount::Team::Player1 ? IsKeyPress(InputPlayer1::Left) : IsKeyPress(InputPlayer2::Left))	m_tParam.pos.x += 1.0f;
		if (pBallCount->GetDefenseTeam() == CBallCount::Team::Player1 ? IsKeyPress(InputPlayer1::Right)	: IsKeyPress(InputPlayer2::Right))	m_tParam.pos.x -= 1.0f;
		if (pBallCount->GetDefenseTeam() == CBallCount::Team::Player1 ? IsKeyPress(InputPlayer1::Up)	: IsKeyPress(InputPlayer2::Up))		m_tParam.pos.y += 1.0f;
		if (pBallCount->GetDefenseTeam() == CBallCount::Team::Player1 ? IsKeyPress(InputPlayer1::Down)	: IsKeyPress(InputPlayer2::Down))	m_tParam.pos.y -= 1.0f;

		// 移動補正
		DirectX::XMFLOAT2 fStrikeZonePos = m_pStrikeZone->GetPos();
		DirectX::XMFLOAT2 fStrikeZoneSize = m_pStrikeZone->GetSize();
		if (m_tParam.pos.x >= fStrikeZonePos.x + fStrikeZoneSize.x / 1.3f) m_tParam.pos.x = fStrikeZonePos.x + fStrikeZoneSize.x / 1.3f;
		if (m_tParam.pos.x <= fStrikeZonePos.x - fStrikeZoneSize.x / 1.3f) m_tParam.pos.x = fStrikeZonePos.x - fStrikeZoneSize.x / 1.3f;
		if (m_tParam.pos.y >= fStrikeZonePos.y + fStrikeZoneSize.y / 1.3f) m_tParam.pos.y = fStrikeZonePos.y + fStrikeZoneSize.y / 1.3f;
		if (m_tParam.pos.y <= fStrikeZonePos.y - fStrikeZoneSize.y / 1.3f) m_tParam.pos.y = fStrikeZonePos.y - fStrikeZoneSize.y / 1.3f;
	}

	// コリジョン情報更新
	m_Collision.square.pos =	m_tParam.pos;
	m_Collision.square.size =	m_tParam.size;
	m_Collision.circle.pos =	m_tParam.pos;
	m_Collision.circle.radius = m_tParam.size.x / 2.0f;
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
}

void CPitchingCursor::SetStrikeZone(CStrikeZone* zone)
{
	m_pStrikeZone.reset(zone);
}

DirectX::XMFLOAT2 CPitchingCursor::GetPos()
{
	return m_tParam.pos;
}

DirectX::XMFLOAT2 CPitchingCursor::GetSize()
{
	return m_tParam.size;
}

void CPitchingCursor::SetPos(DirectX::XMFLOAT2 pos)
{
	m_tParam.pos = pos;
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

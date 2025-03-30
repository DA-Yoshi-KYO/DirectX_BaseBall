#include "Batting.h"
#include "Sprite.h"
#include "Main.h"
#include "Camera.h"
#include "Input.h"
#include "SceneGame.h"
#include "ImGuiManager.h"
#include "BattingCursor.h"
#include "BallCount.h"

Collision::Info2D CBattingCursor::m_Collision = {};

CBattingCursor::CBattingCursor()
	: m_pTexture{ nullptr }, m_pStrikeZone(nullptr)
	, m_bMove(true)
{
	m_pTexture = std::make_unique<Texture>();
	m_tParam.pos = ce_fBattingCursorPos;
	m_tParam.size = DirectX::XMFLOAT2(50.0f, 50.0f);
	m_tParam.rotate = 0.0f;
	m_tParam.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tParam.uvPos = DirectX::XMFLOAT2(0.0f, 0.0f);
	m_tParam.uvSize = DirectX::XMFLOAT2(1.0f, 1.0f);
	m_tParam.world = CCamera::Get2DWolrdMatrix();
	m_tParam.view = CCamera::Get2DViewMatrix();
	m_tParam.proj = CCamera::Get2DProjectionMatrix();

	if (FAILED(m_pTexture->Create(TEXPASS("Cursor.png")))) MessageBox(NULL, "Cursor.png", "Error", MB_OK);


	m_Collision.type = Collision::Type2D::eCircle;
	m_Collision.square.pos = m_tParam.pos;
	m_Collision.square.size = m_tParam.size;
	m_Collision.circle.pos = m_tParam.pos;
	m_Collision.circle.radius = m_tParam.size.x;
}

CBattingCursor::~CBattingCursor()
{
	m_pStrikeZone.release();
}

void CBattingCursor::Update()
{
	CBallCount* pBallCount = CBallCount::GetInstance().get();

	if (m_bMove)
	{
		if (pBallCount->GetOffenseTeam() == CBallCount::Team::Player1 ? IsKeyPress(InputPlayer1::Right) : IsKeyPress(InputPlayer2::Right))	m_tParam.pos.x += 1.0f;
		if (pBallCount->GetOffenseTeam() == CBallCount::Team::Player1 ? IsKeyPress(InputPlayer1::Left) : IsKeyPress(InputPlayer2::Left))	m_tParam.pos.x -= 1.0f;
		if (pBallCount->GetOffenseTeam() == CBallCount::Team::Player1 ? IsKeyPress(InputPlayer1::Up) : IsKeyPress(InputPlayer2::Up))		m_tParam.pos.y += 1.0f;
		if (pBallCount->GetOffenseTeam() == CBallCount::Team::Player1 ? IsKeyPress(InputPlayer1::Down) : IsKeyPress(InputPlayer2::Down))	m_tParam.pos.y -= 1.0f;

		DirectX::XMFLOAT2 fStrikeZonePos = m_pStrikeZone->GetPos();
		DirectX::XMFLOAT2 fStrikeZoneSize = m_pStrikeZone->GetSize();

		if (m_tParam.pos.x >= fStrikeZonePos.x + fStrikeZoneSize.x / 2.0f) m_tParam.pos.x = fStrikeZonePos.x + fStrikeZoneSize.x / 2.0f;
		if (m_tParam.pos.x <= fStrikeZonePos.x - fStrikeZoneSize.x / 2.0f) m_tParam.pos.x = fStrikeZonePos.x - fStrikeZoneSize.x / 2.0f;
		if (m_tParam.pos.y >= fStrikeZonePos.y + fStrikeZoneSize.y / 2.0f) m_tParam.pos.y = fStrikeZonePos.y + fStrikeZoneSize.y / 2.0f;
		if (m_tParam.pos.y <= fStrikeZonePos.y - fStrikeZoneSize.y / 2.0f) m_tParam.pos.y = fStrikeZonePos.y - fStrikeZoneSize.y / 2.0f;
	}
	m_Collision.square.pos = m_tParam.pos;
	m_Collision.square.size = m_tParam.size;
	m_Collision.circle.pos = m_tParam.pos;
	m_Collision.circle.radius = m_tParam.size.x / 4.0f;
}

void CBattingCursor::Draw()
{
#ifdef _COLLISION_DEBUG

	static bool bDebug[2] = {false};

	ApperCollisionSquare(m_Collision.square.pos, m_Collision.square.size,"BattingSquare",&bDebug[0]);
	ApperCollisionCircle(m_Collision.circle.pos, m_Collision.circle.radius, "BattingCircle", &bDebug[1], {220.0f,20.0f});

#endif // _COLLISION_DEBUG

	SetRender2D();
	Sprite::SetParam(m_tParam);
	Sprite::SetTexture(m_pTexture.get());
	Sprite::Draw();
}

void CBattingCursor::SetStrikeZone(CStrikeZone* zone)
{
	m_pStrikeZone.reset(zone);
}

DirectX::XMFLOAT2 CBattingCursor::GetPos()
{
	return m_tParam.pos;
}

DirectX::XMFLOAT2 CBattingCursor::GetSize()
{
	return m_tParam.size;
}

void CBattingCursor::SetPos(DirectX::XMFLOAT2 pos)
{
	m_tParam.pos = pos;
}

void CBattingCursor::SetMove(bool isMove)
{
	m_bMove = isMove;
}

Collision::Info2D CBattingCursor::GetCollision(bool isCursorOnry, Collision::Type2D type)
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

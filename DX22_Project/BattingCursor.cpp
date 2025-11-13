// ==============================
//    インクルード部
// ==============================
#include "Batting.h"
#include "Sprite.h"
#include "Main.h"
#include "Camera.h"
#include "Input.h"
#include "ImGuiManager.h"
#include "BattingCursor.h"
#include "BallCount.h"
#include "Controller.h"

// ==============================
//    静的変数の初期化
// ==============================
Collision::Info2D CBattingCursor::m_Collision = {};

CBattingCursor::CBattingCursor()
	: m_pTexture{ nullptr }, m_pStrikeZone(nullptr)
	, m_bMove(true)
{
	// テクスチャの読み込み
	m_pTexture = std::make_unique<Texture>();
	if (FAILED(m_pTexture->Create(PATH_TEX("Cursor.png")))) MessageBox(NULL, "Cursor.png", "Error", MB_OK);

	// テクスチャパラメータの初期化
	m_tParam.pos = ce_fBattingCursorPos;
	m_tParam.offsetPos = { 0.0f,0.0f };
	m_tParam.size = DirectX::XMFLOAT2(50.0f, 50.0f);
	m_tParam.rotate = 0.0f;
	m_tParam.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tParam.uvPos = DirectX::XMFLOAT2(0.0f, 0.0f);
	m_tParam.uvSize = DirectX::XMFLOAT2(1.0f, 1.0f);
	m_tParam.world = CCamera::Get2DWolrdMatrix(m_tParam.pos, m_tParam.rotate);
	m_tParam.view = CCamera::Get2DViewMatrix();
	m_tParam.proj = CCamera::Get2DProjectionMatrix();

	// コリジョン情報の初期化
	m_Collision = {};
	m_Collision.type = Collision::Type2D::eCircle;
	m_Collision.square.pos = m_tParam.pos;
	m_Collision.square.size = m_tParam.size;
	m_Collision.circle.pos = m_tParam.pos;
	m_Collision.circle.radius = m_tParam.size.x;
}

CBattingCursor::~CBattingCursor()
{
	// コンポジションインスタンスの放棄
	m_pStrikeZone.release();
}

void CBattingCursor::Update()
{
	CGameManager* pBallCount = CGameManager::GetInstance().get();

	// カーソル移動可能なときに移動処理をする
	if (m_bMove)
	{
		// 移動処理
		DirectX::XMFLOAT2 fStrikeZonePos = m_pStrikeZone->GetPos();
		DirectX::XMFLOAT2 fStrikeZoneSize = m_pStrikeZone->GetSize();
		DirectX::XMFLOAT2 fInput = pBallCount->GetOffenseTeam() == CGameManager::Team::Player1 ? CGetLStick((int)CGameManager::Team::Player1) : CGetLStick((int)CGameManager::Team::Player2);
		DirectX::XMFLOAT2 fMaxPos = { fabsf(fStrikeZonePos.x) + fStrikeZoneSize.x / 1.5f , fabsf(fStrikeZonePos.y) + fStrikeZoneSize.y / 1.5f };
		m_tParam.pos = { fStrikeZonePos.x + fInput.x * fMaxPos.x,fStrikeZonePos.y + fInput.y * fMaxPos.y };

		// 移動補正
		if (m_tParam.pos.x >= fStrikeZonePos.x + fStrikeZoneSize.x / 2.0f) m_tParam.pos.x = fStrikeZonePos.x + fStrikeZoneSize.x / 2.0f;
		if (m_tParam.pos.x <= fStrikeZonePos.x - fStrikeZoneSize.x / 2.0f) m_tParam.pos.x = fStrikeZonePos.x - fStrikeZoneSize.x / 2.0f;
		if (m_tParam.pos.y >= fStrikeZonePos.y + fStrikeZoneSize.y / 2.0f) m_tParam.pos.y = fStrikeZonePos.y + fStrikeZoneSize.y / 2.0f;
		if (m_tParam.pos.y <= fStrikeZonePos.y - fStrikeZoneSize.y / 2.0f) m_tParam.pos.y = fStrikeZonePos.y - fStrikeZoneSize.y / 2.0f;
	}

	// コリジョン情報更新
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
	m_tParam.world = CCamera::Get2DWolrdMatrix(m_tParam.pos, m_tParam.rotate);
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

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
#include "SpriteRenderer.h"

CBattingCursor::CBattingCursor()
	: m_bMove(true)
{

}

CBattingCursor::~CBattingCursor()
{
}

void CBattingCursor::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("Cursor.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite"));

	// テクスチャパラメータの初期化
	m_tParam.m_f3Pos = ce_fBattingCursorPos;
	m_tParam.m_f3Size = DirectX::XMFLOAT3(50.0f, 50.0f,0.0f);
	m_tParam.m_f3Rotate = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_tParam.m_f4Color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tParam.m_f2UVPos = DirectX::XMFLOAT2(0.0f, 0.0f);
	m_tParam.m_f2UVSize = DirectX::XMFLOAT2(1.0f, 1.0f);
}

void CBattingCursor::Update()
{
	CScene* pScene = GetScene();
	CStrikeZone* pStrikeZone = pScene->GetGameObject<CStrikeZone>();

	// カーソル移動可能なときに移動処理をする
	if (m_bMove)
	{
		// 移動処理
		DirectX::XMFLOAT3 fStrikeZonePos = pStrikeZone->GetPos();
		DirectX::XMFLOAT3 fStrikeZoneSize = pStrikeZone->GetSize();
		DirectX::XMFLOAT2 fInput = pBallCount->GetOffenseTeam() == CGameManager::Team::Player1 ? CGetLStick((int)CGameManager::Team::Player1) : CGetLStick((int)CGameManager::Team::Player2);
		DirectX::XMFLOAT2 fMaxPos = { fabsf(fStrikeZonePos.x) + fStrikeZoneSize.x / 1.5f , fabsf(fStrikeZonePos.y) + fStrikeZoneSize.y / 1.5f };
		m_tParam.m_f3Pos = { fStrikeZonePos.x + fInput.x * fMaxPos.x,fStrikeZonePos.y + fInput.y * fMaxPos.y,0.0f };

		// 移動補正
		if (m_tParam.m_f3Pos.x >= fStrikeZonePos.x + fStrikeZoneSize.x / 2.0f) m_tParam.m_f3Pos.x = fStrikeZonePos.x + fStrikeZoneSize.x / 2.0f;
		if (m_tParam.m_f3Pos.x <= fStrikeZonePos.x - fStrikeZoneSize.x / 2.0f) m_tParam.m_f3Pos.x = fStrikeZonePos.x - fStrikeZoneSize.x / 2.0f;
		if (m_tParam.m_f3Pos.y >= fStrikeZonePos.y + fStrikeZoneSize.y / 2.0f) m_tParam.m_f3Pos.y = fStrikeZonePos.y + fStrikeZoneSize.y / 2.0f;
		if (m_tParam.m_f3Pos.y <= fStrikeZonePos.y - fStrikeZoneSize.y / 2.0f) m_tParam.m_f3Pos.y = fStrikeZonePos.y - fStrikeZoneSize.y / 2.0f;
	}

	// コリジョン情報更新
	//m_Collision.square.pos = m_tParam.m_f3Pos;
	//m_Collision.square.size = m_tParam.size;
	//m_Collision.circle.pos = m_tParam.m_f3Pos;
	//m_Collision.circle.radius = m_tParam.size.x / 4.0f;
}

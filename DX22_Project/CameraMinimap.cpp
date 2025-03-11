#include "CameraMinimap.h"

CCameraMinimap::CCameraMinimap()
	: m_pPlayer(nullptr) 
{
	m_aspect = 1.0f;
}

CCameraMinimap::~CCameraMinimap()
{
}

void CCameraMinimap::Update()
{
	// プレイヤーが設定されてない時は処理を行わない
	if (!m_pPlayer) { return; }

	// プレイヤーの上空にカメラを設定
	m_look = m_pPlayer->GetPos();
	m_pos = m_look;
	m_pos.y = m_look.y + 200.0f;

	// 真上から見下ろすため、アップベクトルはZ方向
	m_up = { 0.0f, 0.0f, -1.0f };
}

void CCameraMinimap::SetPlayer(CPlayer* pPlayer)
{
	// プレイヤー情報を設定
	m_pPlayer = pPlayer;
}

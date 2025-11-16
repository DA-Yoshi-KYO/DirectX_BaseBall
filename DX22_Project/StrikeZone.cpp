// ==============================
//    インクルード部
// ==============================
#include "StrikeZone.h"
#include "Camera.h"
#include "Sprite.h"
#include "Main.h"

CStrikeZone::CStrikeZone()
{
	if (FAILED(m_pTexture->Create(PATH_TEX("StrikeZone.png")))) MessageBox(NULL, "StrikeZone.png", "Error", MB_OK);

	// パラメータの初期化
	m_tParam.pos = { 0.0f,-100.0f };
	m_tParam.offsetPos = { 0.0f,0.0f };
	m_tParam.size = ce_fStrikeZoneSize;
	m_tParam.rotate = 0.0f;
	m_tParam.color = { 1.0f,1.0f,1.0f,1.0f };
	m_tParam.uvPos = { 0.0f,0.0f };
	m_tParam.uvSize = { 1.0f,1.0f };

	// 当たり判定情報の初期化
	m_Collision.type = Collision::Type2D::eSquare;
	m_Collision.square.pos = m_tParam.pos;
	m_Collision.square.size = m_tParam.size;
}

CStrikeZone::~CStrikeZone()
{
}

void CStrikeZone::Init()
{

}

void CStrikeZone::Update()
{
	// 当たり判定情報の更新
	m_Collision.square.pos = m_tParam.pos;
	m_Collision.square.size = m_tParam.size;
}

Collision::Info2D CStrikeZone::GetCollision()
{
	return m_Collision;
}

// ==============================
//    インクルード部
// ==============================
#include "StrikeZone.h"
#include "Camera.h"
#include "Sprite.h"
#include "Main.h"
#include "SpriteRenderer.h"

CStrikeZone::CStrikeZone()
	: m_Collision{}
{
	// パラメータの初期化
	m_tParam.m_f3Pos = { 0.0f,-100.0f,0.0f };
	m_tParam.m_f3Size = ce_fStrikeZoneSize;
	m_tParam.m_f3Rotate = { 0.0f,0.0f,0.0f };
	m_tParam.m_f4Color = { 1.0f,1.0f,1.0f,1.0f };
	m_tParam.m_f2UVPos = { 0.0f,0.0f };
	m_tParam.m_f2UVSize = { 1.0f,1.0f };

	// 当たり判定情報の初期化
	//m_Collision.type = Collision::Type2D::eSquare;
	//m_Collision.square.pos = m_tParam.pos;
	//m_Collision.square.size = m_tParam.size;
}

CStrikeZone::~CStrikeZone()
{
}

void CStrikeZone::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("StrikeZone.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite.cso"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite.cso"));
}

void CStrikeZone::Update()
{
//	// 当たり判定情報の更新
//	m_Collision.square.pos = m_tParam.pos;
//	m_Collision.square.size = m_tParam.size;
}

Collision::Info2D CStrikeZone::GetCollision()
{
	return m_Collision;
}

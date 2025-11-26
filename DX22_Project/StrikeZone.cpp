#include "StrikeZone.h"
#include "SpriteRenderer.h"

CStrikeZone::CStrikeZone()
	: m_Collision{}
{
	// パラメータの初期化
	m_tParam.m_f3Pos = { SCREEN_WIDTH / 2.0f,450.0f,0.0f };
	m_tParam.m_f3Size = ce_fStrikeZoneSize;
	m_tParam.m_f3Rotate = { 0.0f,0.0f,0.0f };
	m_tParam.m_f4Color = { 1.0f,1.0f,1.0f,1.0f };
	m_tParam.m_f2UVPos = { 0.0f,0.0f };
	m_tParam.m_f2UVSize = { 1.0f,1.0f };

	// 当たり判定情報の初期化
	m_Collision.type = Collision::Type2D::eSquare;
	m_Collision.square.pos = m_tParam.m_f3Pos;
	m_Collision.square.size = m_tParam.m_f3Size;
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
	// 当たり判定情報の更新
	m_Collision.square.pos = m_tParam.m_f3Pos;
	m_Collision.square.size = m_tParam.m_f3Size;
}

Collision::Info2D CStrikeZone::GetCollision()
{
	return m_Collision;
}

#include "TitleSelectCursor.h"
#include "SpriteRenderer.h"

CTitleSelectCursor::CTitleSelectCursor()
	: CGameObject()
	, m_nIndex(0)
{

}

CTitleSelectCursor::~CTitleSelectCursor()
{

}

void CTitleSelectCursor::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("PositionSeeat.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite.cso"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite.cso"));

	m_bActive = false;
}

void CTitleSelectCursor::Update()
{
	if (!m_bActive) return;

	m_tParam.m_f3Pos.y = -180.0f + -120.0f * m_nIndex;

	CGameObject::Update();
}

void CTitleSelectCursor::Draw()
{
	if (!m_bActive) return;

	CGameObject::Draw();
}

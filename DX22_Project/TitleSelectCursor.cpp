#include "TitleSelectCursor.h"
#include "SpriteRenderer.h"

CTitleSelectCursor::CTitleSelectCursor()
	: CGameObject()
	, m_nIndex(0)
{
	m_tParam.m_f3Pos = DirectX::XMFLOAT3(SCREEN_WIDTH * 0.5f - 160.0f, SCREEN_HEIGHT * 0.5f, 0.0f);
	m_tParam.m_f3Size = DirectX::XMFLOAT3(90.0f, 90.0f, 0.0f);
	m_tParam.m_f2UVPos = DirectX::XMFLOAT2(3.0f / 5.0f, 2.0f / 5.0f);
	m_tParam.m_f2UVSize = DirectX::XMFLOAT2(1.0f / 5.0f, 1.0f / 5.0f);
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

	m_tParam.m_f3Pos.y = SCREEN_HEIGHT * 0.5f + 180.0f + 120.0f * m_nIndex;

	CGameObject::Update();
}

void CTitleSelectCursor::Draw()
{
	if (!m_bActive) return;

	CGameObject::Draw();
}

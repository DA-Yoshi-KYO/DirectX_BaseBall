#include "TitleBackGround.h"
#include "SpriteRenderer.h"

CTitleBackGround::CTitleBackGround()
	: CGameObject()
{
	m_tParam.m_f3Size = DirectX::XMFLOAT3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
}

CTitleBackGround::~CTitleBackGround()
{

}

void CTitleBackGround::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("TitleBack.jpg"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite"));
}

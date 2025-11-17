#include "TitleBall.h"
#include "SpriteRenderer.h"

CTitleBall::CTitleBall()
	: CGameObject()
{
	m_tParam.m_f3Size = DirectX::XMFLOAT3(300.0f,300.0f,0.0f);
}

CTitleBall::~CTitleBall()
{

}

void CTitleBall::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("Ball.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite"));
}

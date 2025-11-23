#include "MemberSelectBackGround.h"
#include "SpriteRenderer.h"

CMemberSelectBackGround::CMemberSelectBackGround()
{
	m_tParam.m_f3Pos = DirectX::XMFLOAT3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
	m_tParam.m_f3Size = DirectX::XMFLOAT3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
}

CMemberSelectBackGround::~CMemberSelectBackGround()
{

}

void CMemberSelectBackGround::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("MemberSelectBack.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite.cso"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite.cso"));
}

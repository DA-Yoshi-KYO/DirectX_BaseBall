#include "TeamSelectBackGround.h"
#include "SpriteRenderer.h"

CTeamSelectBackGround::CTeamSelectBackGround()
{
	m_tParam.m_f3Size = DirectX::XMFLOAT3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
}

CTeamSelectBackGround::~CTeamSelectBackGround()
{

}

void CTeamSelectBackGround::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("TeamSelectBack.jpg"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite"));
}

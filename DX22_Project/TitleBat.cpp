#include "TitleBat.h"
#include "SpriteRenderer.h"

CTitleBat::CTitleBat()
	: CGameObject()
{

}

CTitleBat::~CTitleBat()
{

}

void CTitleBat::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("Bat.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite"));
}

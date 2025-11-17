#include "TitleLogo.h"
#include "SpriteRenderer.h"

CTitleLogo::CTitleLogo()
{

}

CTitleLogo::~CTitleLogo()
{

}

void CTitleLogo::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("TitleLogo.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite"));
}

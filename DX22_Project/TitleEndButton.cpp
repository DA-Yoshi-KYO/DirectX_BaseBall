#include "TitleEndButton.h"
#include "SpriteRenderer.h"

CTitleEndButton::CTitleEndButton()
	: CAnimationObject()
{

}

CTitleEndButton::~CTitleEndButton()
{

}

void CTitleEndButton::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("EndButton.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite"));
}

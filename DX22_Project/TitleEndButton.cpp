#include "TitleEndButton.h"
#include "SpriteRenderer.h"

TitleEndButton::TitleEndButton()
{

}

TitleEndButton::~TitleEndButton()
{

}

void TitleEndButton::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("EndButton.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite"));
}

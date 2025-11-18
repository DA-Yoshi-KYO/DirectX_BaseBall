#include "TitleStartButton.h"
#include "SpriteRenderer.h"

CTitleStartButton::CTitleStartButton()
	: CAnimationObject()
{

}

CTitleStartButton::~CTitleStartButton()
{

}

void CTitleStartButton::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("StartButton.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite"));
}

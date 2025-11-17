#include "TitleStartButton.h"
#include "SpriteRenderer.h"

TitleStartButton::TitleStartButton()
{

}

TitleStartButton::~TitleStartButton()
{

}

void TitleStartButton::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("StartButton.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite"));
}

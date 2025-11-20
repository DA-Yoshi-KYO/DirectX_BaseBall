#include "PositionIcon.h"
#include "SpriteRenderer.h"

CPositionIcon::CPositionIcon()
	: CGameObject()
{

}

CPositionIcon::~CPositionIcon()
{

}

void CPositionIcon::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("PositionSeeat.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite"));
}

void CPositionIcon::Init(Positions position)
{
	
}
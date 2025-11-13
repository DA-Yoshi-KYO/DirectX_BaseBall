#include "StrikeCount.h"
#include "SpriteRenderer.h"

CStrikeCount::CStrikeCount()
	: CCountObject()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load("BallCountSheet");
	pRenderer->LoadPixelShader("");
}

CStrikeCount::~CStrikeCount()
{
	
}


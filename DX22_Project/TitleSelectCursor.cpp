#include "TitleSelectCursor.h"
#include "SpriteRenderer.h"

CTitleSelectCursor::CTitleSelectCursor()
	: CGameObject()
{

}

CTitleSelectCursor::~CTitleSelectCursor()
{

}

void CTitleSelectCursor::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("PositionSeeat.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite"));
}

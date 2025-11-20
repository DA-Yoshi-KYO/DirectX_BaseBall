#include "MemberSelectCursor.h"
#include "SpriteRenderer.h"

CMemberSelectCursor::CMemberSelectCursor()
	: CGameObject()
{

}

CMemberSelectCursor::~CMemberSelectCursor()
{

}

void CMemberSelectCursor::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("Ball.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite"));
}

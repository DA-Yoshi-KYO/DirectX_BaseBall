#include "ReleasePoint.h"
#include "SpriteRenderer.h"

constexpr int ce_nSheetSplit = 5;

CReleasePoint::CReleasePoint()
{
	m_tParam.m_f4Color = {0.0f,0.0f,1.0f,0.5f};
	m_tParam.m_f2UVPos = { 1.0f / (float)ce_nSheetSplit,2.0f / (float)ce_nSheetSplit };
	m_tParam.m_f2UVSize = { 1.0f / (float)ce_nSheetSplit,1.0f / (float)ce_nSheetSplit };
	m_f3Future = m_tParam.m_f3Pos;
}

CReleasePoint::~CReleasePoint()
{
}

void CReleasePoint::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>(); 
	pRenderer->Load(PATH_TEX("BallCountSheet.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite.cso"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite.cso"));
}

void CReleasePoint::Update()
{
	m_tParam.m_f3Pos = m_f3Future;

	CGameObject::Update();
}

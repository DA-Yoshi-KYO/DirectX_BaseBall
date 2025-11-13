#include "Inning.h"
#include "SpriteRenderer.h"

CInning::CInning()
	: m_nInningNum(1), m_bIsStart(true)
{

}

CInning::~CInning()
{

}

void CInning::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("BallCountSheet.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite"));
}

void CInning::InningProgress()
{
	if (!m_bIsStart)
	{
		m_nInningNum++;
	}

	m_bIsStart ^= true;
}

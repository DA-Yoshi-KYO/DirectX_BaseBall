#include "Score.h"
#include "SpriteRenderer.h"

#undef min

constexpr int ce_nMaxScore = 99;

CScore::CScore()
	: m_nScore(0)
{

}

CScore::~CScore()
{

}

void CScore::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("BallCountSheet.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite"));
}

void CScore::AddScore()
{
	m_nScore++;
	m_nScore = std::min(m_nScore, ce_nMaxScore);
}
#include "ScoreBoard.h"
#include "SpriteRenderer.h"

// スコアボードのパラメータ
constexpr DirectX::XMFLOAT3 ce_fBackPos = { 480.0f,260.0f,0.0f };
constexpr DirectX::XMFLOAT3 ce_fBackSize = { 280.0f,170.0f,0.0f };

CScoreBoard::CScoreBoard()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>(); 
	pRenderer->Load(PATH_TEX("BallCount.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite.cso"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite.cso"));

	m_tParam.m_f3Pos = ce_fBackPos;
	m_tParam.m_f3Size = ce_fBackSize;
}

CScoreBoard::~CScoreBoard()
{

}

void CScoreBoard::Init()
{

}

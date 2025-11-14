#include "Score.h"
#include "SpriteRenderer.h"

#undef min

constexpr int ce_nMaxScore = 99;
constexpr DirectX::XMFLOAT3 ce_fScoreSize = { 50.0f,50.0f,0.0f };
constexpr DirectX::XMFLOAT2 ce_fScoreAjust = { 30.0f,60.0f };

CScore::CScore()
	: m_nScore(0), m_f2ScoreUVPos{}, m_f3ScoreOnePos{}
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

	m_tParam.m_f3Size = ce_fScoreSize;
}

void CScore::Update()
{
	int nNum = 0;

	// 1‚ÌˆÊ
	nNum = m_nScore % 10;
	m_f2ScoreUVPos[0] = DirectX::XMFLOAT2((nNum % 5) * m_tParam.m_f2UVSize.x, (nNum / 5) * m_tParam.m_f2UVSize.y);
	// 10‚ÌˆÊ
	nNum = m_nScore / 10;
	m_f2ScoreUVPos[1] = DirectX::XMFLOAT2((nNum % 5) * m_tParam.m_f2UVSize.x, (nNum / 5) * m_tParam.m_f2UVSize.y);
}

void CScore::Draw()
{
	for (int i = 0; i < 2; i++)
	{
		if (i == 1 && m_nScore < 10) continue;
		m_tParam.m_f3Pos = { m_f3ScoreOnePos.x - ce_fScoreAjust.x * i, m_f3ScoreOnePos.y,0.0f };
		m_tParam.m_f2UVPos = m_f2ScoreUVPos[i];
		CGameObject::Draw();
	}
}

void CScore::AddScore()
{
	m_nScore++;
	m_nScore = std::min(m_nScore, ce_nMaxScore);
}
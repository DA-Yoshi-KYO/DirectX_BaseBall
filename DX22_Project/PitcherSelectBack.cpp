#include "PitcherSelectBack.h"
#include "SpriteRenderer.h"

constexpr DirectX::XMFLOAT3 ce_fBenchSize = { 1320.0f,720.0f,0.0f };

CPitcherSelectBack::CPitcherSelectBack()
	: CGameObject()
{
	m_tParam.m_f3Size = ce_fBenchSize;
}

CPitcherSelectBack::~CPitcherSelectBack()
{

}

void CPitcherSelectBack::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("BenchPitcher.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite.cso"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite.cso"));
}

#include "PitchingCircle.h"
#include "SpriteRenderer.h"


constexpr float ce_fCircleTime = 0.5f;		// ピッチングサークルが縮むまでの時間(秒)

CPitchingCircle::CPitchingCircle()
	: CGameObject()
	, m_bScaleDown(false)
{
	m_tParam.m_f3Size = ce_fPitchingCircleFirstSize;
	m_tParam.m_f4Color = { 1.0f,1.0f,1.0f,0.5f };
	m_f3Future = m_tParam.m_f3Pos;
}

CPitchingCircle::~CPitchingCircle()
{

}

void CPitchingCircle::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("PitchingCircle.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite.cso"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite.cso"));
}

void CPitchingCircle::Update()
{
	m_tParam.m_f3Pos = m_f3Future;
	if (m_bScaleDown)
	{
		m_tParam.m_f3Size.x -= (ce_fPitchingCircleFirstSize.x - ce_fPitchingCircleEndSize.x) / (ce_fCircleTime * fFPS);
		m_tParam.m_f3Size.y -= (ce_fPitchingCircleFirstSize.y - ce_fPitchingCircleEndSize.y) / (ce_fCircleTime * fFPS);
	}
	CGameObject::Update();
}

void CPitchingCircle::ResetInitSize()
{
	m_tParam.m_f3Size = ce_fPitchingCircleFirstSize;
}

void CPitchingCircle::StartScaleDown()
{
	if (m_bScaleDown) return;
	m_bScaleDown = true;
}

void CPitchingCircle::Pitched()
{
	m_bScaleDown = false;
}

#include "EffectBase.h"
#include "Oparation.h"

CEffectBase::CEffectBase()
	: m_tParticleParam{}, m_pRenderer(nullptr)
{

}

CEffectBase::~CEffectBase()
{

}

void CEffectBase::Init()
{
	m_pRenderer = AddComponent<CSpriteRenderer>();
	m_pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite.cso"));
	m_pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite.cso"));
}

void CEffectBase::Update()
{
	//‚·‚×‚ÄI—¹‚µ‚Ä‚é‚©”»’è
	bool End = true;

	for (int i = 0; i < m_tParticleParam.size(); ++i)
	{
		m_tParticleParam[i].m_fTime += 1.0f / fFPS;
		if (!m_tParticleParam[i].m_bUse)continue;

		End = false;

		if (m_tParticleParam[i].m_fTime < 0.0f) continue;

		m_tParticleParam[i].m_fTime += 1.0f / fFPS;

		m_tParticleParam[i].m_f3Pos += m_tParticleParam[i].m_f3Velocity;

		if (m_tParticleParam[i].m_fTime > m_tParticleParam[i].m_fDurationTime)
		{
			m_tParticleParam[i].m_bUse = false;
		}
	}

	//‚·‚×‚ÄI—¹‚µ‚½‚çíœ
	if (End)
	{
		Destroy();
	}
}

void CEffectBase::Draw()
{
	m_pRenderer = GetComponent<CSpriteRenderer>();

	for (const auto& itr : m_tParticleParam)
	{
		if (!itr.m_bUse) continue;
		if (itr.m_fTime < 0.0f) continue;

		m_pRenderer->SetPos(itr.m_f3Pos);
		m_pRenderer->SetSize(itr.m_f3Size);
		m_pRenderer->SetColor(itr.m_f4Color);
		m_pRenderer->Draw();
	}
}

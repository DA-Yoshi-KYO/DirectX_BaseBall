#include "2DEffectBase.h"
#include"Shader.h"
#include"SpriteRenderer.h"

void C2DEffectBase::Init()
{
	CEffectBase::Init();

	CSpriteRenderer* pSpriteRenderer = AddComponent<CSpriteRenderer>();
	pSpriteRenderer->Init(DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f),TEX_PATH("Particle.png"), Origin::Center);

	m_tRendererParam.m_fPos = DirectX::XMFLOAT3(400.0f, 525.0f, 0.0f);
	m_tRendererParam.m_fSize = DirectX::XMFLOAT3(100.0f, 100.0f, 1.0f);
	m_tRendererParam.m_fRotate = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_tRendererParam.m_fUVPos = DirectX::XMFLOAT2(0.0f, 0.0f);
	m_tRendererParam.m_fUVSize = DirectX::XMFLOAT2(1.0f, 1.0f);
	m_tRendererParam.m_fColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_eBlendState = BlendStateKind::AlphaBlend;
}

void C2DEffectBase::Draw()
{
	CSpriteRenderer* pSpriteRenderer = GetComponent<CSpriteRenderer>();

	for (int i = 0; i < m_nMaxParticle; ++i)
	{
		if (!m_tParticleParam[i].bUse)continue;

		m_tRendererParam.m_fPos = m_tParticleParam[i].fPos;
		m_tRendererParam.m_fSize = m_tParticleParam[i].fSize;
		m_tRendererParam.m_fColor = m_tParticleParam[i].fColor;

		pSpriteRenderer->SetRendererParam(m_tRendererParam);

		pSpriteRenderer->Draw();
	}
}

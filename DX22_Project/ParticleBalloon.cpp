#include "ParticleBalloon.h"
#include "Oparation.h"
#include "Main.h"

constexpr int ce_nMaxParticle = 20;

CParticleBalloon::CParticleBalloon()
	: CEffectBase()
{

}

CParticleBalloon::~CParticleBalloon()
{

}

void CParticleBalloon::Init()
{
	CEffectBase::Init();

	m_pRenderer->Load(PATH_TEX("Balloon.png"));

	for (int i = 0; i < ce_nMaxParticle; i++)
	{
		PartcleParam tParam;
		tParam.m_f3Pos = DirectX::XMFLOAT3(SCREEN_WIDTH * 0.8f + GetRandOfRange(-10, 0) * 100.0f,SCREEN_HEIGHT,0.0f);
		tParam.m_f3Size = DirectX::XMFLOAT3(100.0f,100.0f,0.0f);
		tParam.m_f3Velocity = DirectX::XMFLOAT3(GetRandOfRange(50, 0) / 10.0f,-8.0f,0.0f);
		tParam.m_f3CreatePos = tParam.m_f3Pos;
		tParam.m_f4Color = DirectX::XMFLOAT4(1.0f, GetRandOfRange(5, 10) / 10.0f, 0.0f, 0.7f);
		tParam.m_fTime = GetRandOfRange(-30, 0) / 10.0f;
		tParam.m_fDurationTime = 10.0f;
		tParam.m_bUse = true;
		m_tParticleParam.push_back(tParam);
	}
}

void CParticleBalloon::OnDestroy()
{
	GetScene()->AddGameObject<CParticleBalloon>("Balloon", Tag::UI);
}

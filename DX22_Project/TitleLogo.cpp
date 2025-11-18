#include "TitleLogo.h"
#include "SpriteRenderer.h"
#include "Easing.h"
#include "Oparation.h"

constexpr DirectX::XMFLOAT3 ce_fStartLogoSize = DirectX::XMFLOAT3(0.0f, 0.0f,0.0f);
constexpr DirectX::XMFLOAT3 ce_fEndLogoSize = DirectX::XMFLOAT3(1000.0f,600.0f,0.0f);

CTitleLogo::CTitleLogo()
	: CAnimationObject()
{

}

CTitleLogo::~CTitleLogo()
{

}

void CTitleLogo::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("TitleLogo.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite"));

	m_fDurationTime = 0.5f;
}

void CTitleLogo::ExecAnimation()
{
	static float fTime = 0.0f;
	constexpr float easeSizeTime = 0.5f;

	m_tParam.m_f3Size= ce_fStartLogoSize +(ce_fEndLogoSize - ce_fStartLogoSize) *  CEasing::EaseOutBack(fTime, easeSizeTime);
}

void CTitleLogo::EndAnimation()
{
	m_bIsAnimation = false;

	m_tParam.m_f3Size = ce_fEndLogoSize;
}

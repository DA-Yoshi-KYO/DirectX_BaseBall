#include "TitleLogo.h"
#include "SpriteRenderer.h"
#include "Easing.h"

constexpr DirectX::XMFLOAT2 ce_fStartLogoSize = { 0.0f, 0.0f };
constexpr DirectX::XMFLOAT2 ce_fEndLogoSize = { 1000.0f,600.0f };

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

	m_tParam.m_f3Size.x = ce_fStartLogoSize.x +(ce_fEndLogoSize.x - ce_fStartLogoSize.x) *  CEasing::EaseOutBack(fTime, easeSizeTime);
	m_tParam.m_f3Size.y = ce_fStartLogoSize.y +(ce_fEndLogoSize.y - ce_fStartLogoSize.y) *  CEasing::EaseOutBack(fTime, easeSizeTime);
}

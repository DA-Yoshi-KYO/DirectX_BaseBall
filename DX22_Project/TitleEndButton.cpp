#include "TitleEndButton.h"
#include "SpriteRenderer.h"
#include "Easing.h"
#include "Oparation.h"

constexpr DirectX::XMFLOAT3 ce_fStartButtonPos = DirectX::XMFLOAT3(1000.0f,-300.0f, 0.0f);
constexpr DirectX::XMFLOAT3 ce_fEndButtonPos = DirectX::XMFLOAT3(0.0f, -300.0f, 0.0f);

CTitleEndButton::CTitleEndButton()
	: CAnimationObject()
{

}

CTitleEndButton::~CTitleEndButton()
{

}

void CTitleEndButton::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("EndButton.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite"));
}

void CTitleEndButton::ExecAnimation()
{
	const float easePosTimeButton = 0.5f;
	const float easePow = 2.5f;

	m_tParam.m_f3Pos = ce_fStartButtonPos + (ce_fEndButtonPos - ce_fStartButtonPos) * CEasing::EaseOutBack(m_fTime, easePosTimeButton, easePow);
}

void CTitleEndButton::EndAnimation()
{
	m_bIsAnimation = false;

	m_tParam.m_f3Pos = ce_fEndButtonPos;
}

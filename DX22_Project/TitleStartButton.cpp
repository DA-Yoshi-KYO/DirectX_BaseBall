#include "TitleStartButton.h"
#include "SpriteRenderer.h"
#include "Easing.h"
#include "Oparation.h"

constexpr DirectX::XMFLOAT3 ce_fStartButtonPos = DirectX::XMFLOAT3(1000.0f, -180.0f, 0.0f);
constexpr DirectX::XMFLOAT3 ce_fEndButtonPos = DirectX::XMFLOAT3(0.0f, -180.0f, 0.0f);

CTitleStartButton::CTitleStartButton()
	: CAnimationObject()
{

}

CTitleStartButton::~CTitleStartButton()
{

}

void CTitleStartButton::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("StartButton.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite.cso"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite.cso"));
}

void CTitleStartButton::ExecAnimation()
{
	const float easePosTimeButton = 0.5f;
	const float easePow = 2.5f;

	m_tParam.m_f3Pos = ce_fStartButtonPos + (ce_fEndButtonPos - ce_fStartButtonPos) * CEasing::EaseOutBack(m_fTime, easePosTimeButton, easePow);
}

void CTitleStartButton::EndAnimation()
{
	m_bIsAnimation = false;

	m_tParam.m_f3Pos = ce_fEndButtonPos;
}

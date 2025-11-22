#include "TitleBat.h"
#include "SpriteRenderer.h"
#include "Oparation.h"

constexpr DirectX::XMFLOAT3 ce_fStartBatPos = DirectX::XMFLOAT3(0.0f,-800.0f,0.0f);
constexpr DirectX::XMFLOAT3 ce_fEndBatPos = DirectX::XMFLOAT3(0.0f, 20.0f,0.0f);
constexpr float ce_fEndRotate = DirectX::XMConvertToRadians(1870.0f);

CTitleBat::CTitleBat()
	: CAnimationObject()
	, m_bIsRight(true)
{

}

CTitleBat::~CTitleBat()
{

}

void CTitleBat::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("Bat.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite.cso"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite.cso"));

	m_fDurationTime = 0.5f;
}

void CTitleBat::ExecAnimation()
{
	constexpr float ce_fGroveMoveTime = 0.5f;
	constexpr float ce_fGroveMoveX = 200.0f;

	float fRad = m_fTime / ce_fGroveMoveTime * 180.0f;
	fRad = DirectX::XMConvertToRadians(fRad);
	float fCos = sinf(fRad);

	m_tParam.m_f3Pos = ce_fStartBatPos + (ce_fEndBatPos - ce_fStartBatPos) * (m_fTime / ce_fGroveMoveTime);

	if (m_bIsRight) m_tParam.m_f3Rotate.z = 0.0f + (0.0f - ce_fEndRotate) * (m_fTime / ce_fGroveMoveTime);
	else m_tParam.m_f3Rotate.z = 0.0f + (0.0f - -ce_fEndRotate) * (m_fTime / ce_fGroveMoveTime);
}

void CTitleBat::EndAnimation()
{
	m_bIsAnimation = false;

	m_tParam.m_f3Pos = ce_fEndBatPos;
	if (m_bIsRight) m_tParam.m_f3Rotate.z = ce_fEndRotate;
	else m_tParam.m_f3Rotate.z = -ce_fEndRotate;
}

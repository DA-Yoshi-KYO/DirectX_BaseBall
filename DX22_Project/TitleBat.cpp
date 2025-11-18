#include "TitleBat.h"
#include "SpriteRenderer.h"

constexpr DirectX::XMFLOAT2 ce_fStartBatPos = { 0.0f,-800.0f };
constexpr DirectX::XMFLOAT2 ce_fEndBatPos = { 0.0f, 20.0f };

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
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite"));

	m_fDurationTime = 0.5f;
}

void CTitleBat::ExecAnimation()
{
	constexpr float ce_fGroveMoveTime = 0.5f;
	constexpr float ce_fGroveMoveX = 200.0f;

	float fRad = m_fTime / ce_fGroveMoveTime * 180.0f;
	fRad = DirectX::XMConvertToRadians(fRad);
	float fCos = sinf(fRad);

	if (m_bIsRight)
	{
		m_tParam.m_f3Pos.x =
			(ce_fEndBatPos.x - ce_fStartBatPos.x) * (m_fTime / ce_fGroveMoveTime) +
			(fCos * ce_fGroveMoveX) +
			ce_fStartBatPos.x;
		m_tParam.m_f3Pos.y =
			(ce_fEndBatPos.y - ce_fStartBatPos.y) * (m_fTime / ce_fGroveMoveTime) +
			ce_fStartBatPos.y;
		m_tParam.m_f3Rotate.z = (0.0f - DirectX::XMConvertToRadians(1870.0f)) * (m_fTime / ce_fGroveMoveTime);
	}
	else
	{
		m_tParam.m_f3Pos.x =
			(ce_fEndBatPos.x - ce_fStartBatPos.x) * (m_fTime / ce_fGroveMoveTime) +
			(fCos * -ce_fGroveMoveX) +
			ce_fStartBatPos.x;
		m_tParam.m_f3Pos.y =
			(ce_fEndBatPos.y - ce_fStartBatPos.y) * (m_fTime / ce_fGroveMoveTime) +
			ce_fStartBatPos.y;
		m_tParam.m_f3Rotate.z = (0.0f - DirectX::XMConvertToRadians(-1870.0f)) * (m_fTime / ce_fGroveMoveTime);
	}
}

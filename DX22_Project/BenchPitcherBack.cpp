#include "BenchPitcherBack.h"
#include "SpriteRenderer.h"
#include "Defines.h"
#include "Oparation.h"
#include "BenchBatterBack.h"

#undef min

constexpr float ce_fMoveDuration = 0.5f;

DirectX::XMFLOAT3 ce_f3InitBackPos[] =
{
	DirectX::XMFLOAT3(SCREEN_WIDTH + 400.0f,360.0f,0.0f),
	DirectX::XMFLOAT3(-400.0f,360.0f,0.0f)
};

DirectX::XMFLOAT3 ce_f3EndBackPos[] =
{
	DirectX::XMFLOAT3(780.0f,360.0f,0.0f),
	DirectX::XMFLOAT3(500.0f,360.0f,0.0f)
};

CBenchPitcherBack::CBenchPitcherBack()
	: CGameObject()
	, m_bSelectable(false), m_bMove(false)
	, m_fTime(0.0f), m_nPlayerIndex(0)
{

}

CBenchPitcherBack::~CBenchPitcherBack()
{

}

void CBenchPitcherBack::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("BenchPitcher.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite.cso"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite.cso"));
}

void CBenchPitcherBack::Init(int inPlayerIndex)
{
	m_nPlayerIndex = inPlayerIndex;
	m_tParam.m_f3Pos = ce_f3InitBackPos[m_nPlayerIndex];
}

void CBenchPitcherBack::Update()
{
	if (m_bMove)
	{
		m_fTime += 1.0f / fFPS;
		m_fTime = std::min(m_fTime, ce_fMoveDuration);

		if (m_bSelectable) m_tParam.m_f3Pos = ce_f3InitBackPos[m_nPlayerIndex] + (ce_f3EndBackPos[m_nPlayerIndex] - ce_f3InitBackPos[m_nPlayerIndex]) * (m_fTime / ce_fMoveDuration);
		else m_tParam.m_f3Pos = ce_f3EndBackPos[m_nPlayerIndex] + (ce_f3InitBackPos[m_nPlayerIndex] - ce_f3EndBackPos[m_nPlayerIndex]) * (m_fTime / ce_fMoveDuration);

		if (m_fTime >= ce_fMoveDuration)
		{
			m_fTime = 0.0f;
			m_bMove = false;
		}
	}

	CGameObject::Update();
}

void CBenchPitcherBack::SetSelectable(bool isSelect)
{
	if (m_bMove) return;
	if (m_bSelectable == isSelect) return;

	m_bSelectable = isSelect; 
	m_bMove = true;
}

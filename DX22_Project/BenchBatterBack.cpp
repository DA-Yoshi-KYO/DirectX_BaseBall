#include "BenchBatterBack.h"
#include "SpriteRenderer.h"
#include "Defines.h"
#include "Oparation.h"

#undef min

constexpr float ce_fMoveDuration = 0.5f;

DirectX::XMFLOAT3 ce_f3InitPos[] =
{
	DirectX::XMFLOAT3(SCREEN_WIDTH + 400.0f,360.0f,0.0f),
	DirectX::XMFLOAT3(-400.0f,360.0f,0.0f)
};

DirectX::XMFLOAT3 ce_f3EndPos[] =
{
	DirectX::XMFLOAT3(780.0f,360.0f,0.0f),
	DirectX::XMFLOAT3(500.0f,360.0f,0.0f)
};

CBenchBatterBack::CBenchBatterBack()
	: m_bMove(false), m_bSelectable(false)
	, m_fTime(0.0f), m_nPlayerIndex(0)
{

}

CBenchBatterBack::~CBenchBatterBack()
{

}

void CBenchBatterBack::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("BenchBatter.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite.cso"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite.cso"));
}

void CBenchBatterBack::Init(int inPlayerIndex)
{
	m_nPlayerIndex = inPlayerIndex;
	m_tParam.m_f3Pos = ce_f3InitPos[m_nPlayerIndex];
}

void CBenchBatterBack::Update()
{
	if (m_bMove)
	{
		m_fTime += 1.0f / fFPS;
		m_fTime = std::min(m_fTime, ce_fMoveDuration);

		if (m_bSelectable) m_tParam.m_f3Pos = ce_f3InitPos[m_nPlayerIndex] + (ce_f3EndPos[m_nPlayerIndex] - ce_f3InitPos[m_nPlayerIndex]) * (m_fTime / ce_fMoveDuration);
		else m_tParam.m_f3Pos = ce_f3EndPos[m_nPlayerIndex] + (ce_f3InitPos[m_nPlayerIndex] - ce_f3EndPos[m_nPlayerIndex]) * (m_fTime / ce_fMoveDuration);

		if (m_fTime >= ce_fMoveDuration)
		{
			m_fTime = 0.0f;
			m_bMove = false;
		}
	}

	CGameObject::Update();
}

void CBenchBatterBack::SetSelectable(bool isSelect)
{
	if (m_bMove) return;
	if (m_bSelectable == isSelect) return;

	m_bSelectable = isSelect;
	m_bMove = true;
}

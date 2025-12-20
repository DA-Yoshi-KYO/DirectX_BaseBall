#include "TeamSelectArrow.h"
#include "SpriteRenderer.h"

CTeamSelectArrow::CTeamSelectArrow()
	: CGameObject()
{
	m_tParam.m_f3Size = DirectX::XMFLOAT3(90.0f, 90.0f, 0.0f);
	m_tParam.m_f2UVPos = DirectX::XMFLOAT2(2.0f / 5.0f, 2.0f / 5.0f);
	m_tParam.m_f2UVSize = DirectX::XMFLOAT2(1.0f / 5.0f, 1.0f / 5.0f);
}

CTeamSelectArrow::~CTeamSelectArrow()
{

}

void CTeamSelectArrow::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("PositionSeeat.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite.cso"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite.cso"));

}

void CTeamSelectArrow::SetIsUp(bool isUp)
{
	if (isUp)
	{
		m_tParam.m_f3Rotate.z = DirectX::XMConvertToRadians(90.0f);
	}
	else 
	{
		m_tParam.m_f3Rotate.z = DirectX::XMConvertToRadians(-90.0f);
	}
}

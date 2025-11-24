#include "PositionIcon.h"
#include "SpriteRenderer.h"

CPositionIcon::CPositionIcon()
	: CGameObject()
{

}

CPositionIcon::~CPositionIcon()
{

}

void CPositionIcon::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("PositionSeeat.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite.cso"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite.cso"));
}

void CPositionIcon::Init(Positions position)
{
	m_tParam.m_f2UVSize = DirectX::XMFLOAT2(1.0f / 5.0f, 1.0f / 5.0f);
	switch (position)
	{
	case Positions::Pitcher:
		m_tParam.m_f2UVPos = DirectX::XMFLOAT2(0.0f / 5.0f, 0.0f / 5.0f);
		break;
	case Positions::Chatcher:
		m_tParam.m_f2UVPos = DirectX::XMFLOAT2(1.0f / 5.0f, 0.0f / 5.0f);
		break;
	case Positions::First:
		m_tParam.m_f2UVPos = DirectX::XMFLOAT2(2.0f / 5.0f, 0.0f / 5.0f);
		break;
	case Positions::Second:
		m_tParam.m_f2UVPos = DirectX::XMFLOAT2(3.0f / 5.0f, 0.0f / 5.0f);
		break;
	case Positions::Third:
		m_tParam.m_f2UVPos = DirectX::XMFLOAT2(4.0f / 5.0f, 0.0f / 5.0f);
		break;
	case Positions::Short:
		m_tParam.m_f2UVPos = DirectX::XMFLOAT2(0.0f / 5.0f, 1.0f / 5.0f);
		break;
	case Positions::Left:
		m_tParam.m_f2UVPos = DirectX::XMFLOAT2(1.0f / 5.0f, 1.0f / 5.0f);
		break;
	case Positions::Center:
		m_tParam.m_f2UVPos = DirectX::XMFLOAT2(2.0f / 5.0f, 1.0f / 5.0f);
		break;
	case Positions::Right:
		m_tParam.m_f2UVPos = DirectX::XMFLOAT2(3.0f / 5.0f, 1.0f / 5.0f);
		break;
	case Positions::Max:
		m_tParam.m_f2UVPos = DirectX::XMFLOAT2(4.0f / 5.0f, 1.0f / 5.0f);
		break;
	default:
		break;
	}
}
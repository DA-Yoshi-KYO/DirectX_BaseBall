#include "MemberBack.h"
#include "SpriteRenderer.h"

constexpr DirectX::XMINT2 ce_nSplit = DirectX::XMINT2(5, 5);
constexpr DirectX::XMFLOAT3 ce_fLineupSize = { 240.0f,45.0f,0.0f };

CMemberBack::CMemberBack()
{

}

CMemberBack::~CMemberBack()
{

}

void CMemberBack::Init(Positions position)
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("PlayerSeeat.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite"));

	m_tParam.m_f3Size = ce_fLineupSize;
	m_tParam.m_f2UVSize = DirectX::XMFLOAT2(3.0f / ce_nSplit.x, 1.0f / ce_nSplit.y);
	switch (position)
	{
	case Positions::Pitcher:
		m_tParam.m_f2UVPos = DirectX::XMFLOAT2(0.0f / ce_nSplit.x, 3.0f / ce_nSplit.y);
		break;
	case Positions::Chatcher:
		m_tParam.m_f2UVPos = DirectX::XMFLOAT2(0.0f / ce_nSplit.x, 2.0f / ce_nSplit.y);
		break;
	case Positions::First:
	case Positions::Second:
	case Positions::Third:
	case Positions::Short:
		m_tParam.m_f2UVPos = DirectX::XMFLOAT2(0.0f / ce_nSplit.x, 1.0f / ce_nSplit.y);
		break;
	case Positions::Left:
	case Positions::Center:
	case Positions::Right:
		m_tParam.m_f2UVPos = DirectX::XMFLOAT2(0.0f / ce_nSplit.x, 0.0f / ce_nSplit.y);
		break;
	default:
		break;
	}
}

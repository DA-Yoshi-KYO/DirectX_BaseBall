#include "MemberSelectCursor.h"
#include "SpriteRenderer.h"
#include "Oparation.h"

constexpr DirectX::XMFLOAT3 ce_f3BaseSize = DirectX::XMFLOAT3(50.0f, 50.0f, 0.0f);
constexpr float ce_fSubtractScale = 0.2f;
constexpr float ce_fMaxTime = 1.0f;

CMemberSelectCursor::CMemberSelectCursor()
	: CGameObject()
	, m_fTime(0.0f)
{
	m_tParam.m_f3Size = DirectX::XMFLOAT3(50.0f, 50.0f, 0.0f);
}

CMemberSelectCursor::~CMemberSelectCursor()
{

}

void CMemberSelectCursor::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("MemberSelectCursor.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite.cso"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite.cso"));
}

void CMemberSelectCursor::Update()
{
	m_fTime += 1.0f / fFPS;
	float t = m_fTime / ce_fMaxTime;
	float step = fabsf(sinf(DirectX::XMConvertToRadians(t * 180.0f)));
	m_tParam.m_f3Size = ce_f3BaseSize + ce_f3BaseSize * (ce_fSubtractScale * step);
	CGameObject::Update();
}

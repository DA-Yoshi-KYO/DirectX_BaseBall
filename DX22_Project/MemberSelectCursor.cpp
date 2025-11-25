#include "MemberSelectCursor.h"
#include "SpriteRenderer.h"

constexpr float ce_fRotateOfSec = DirectX::XMConvertToRadians(360.0f);

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
	pRenderer->Load(PATH_TEX("Ball.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite.cso"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite.cso"));
}

void CMemberSelectCursor::Update()
{
	m_fTime += 1.0f / fFPS;
	m_tParam.m_f3Rotate.z = ce_fRotateOfSec * m_fTime;
	CGameObject::Update();
}

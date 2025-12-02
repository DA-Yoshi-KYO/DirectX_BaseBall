#include "Runner.h"
#include "ModelRenderer.h"

CRunner::CRunner()
	: CGameObject()
	, m_nNowBase(int(GotBase::None))
	, m_pCollision(nullptr)
{
	m_tParam.m_f3Pos = DirectX::XMFLOAT3(0.0f, -7.0f, -218.0f);
	m_tParam.m_f3Size = DirectX::XMFLOAT3(5.0f, 5.0f, 5.0f);
}

CRunner::~CRunner()
{

}

void CRunner::Init()
{
	CModelRenderer* pRenderer = AddComponent<CModelRenderer>();
	pRenderer->Load(PATH_MODEL("Ball.obj"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Object.cso"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_TexColor.cso"));

	m_pCollision = AddComponent<CCollisionBox>();
	m_pCollision->SetTag("Runner");
	m_pCollision->SetInfo(m_tParam.m_f3Pos, m_tParam.m_f3Size);
}

void CRunner::GoToNextBase()
{
	if (m_nNowBase != int(GotBase::Third))
	{
		m_nNowBase++;
	}
	else Destroy();
}

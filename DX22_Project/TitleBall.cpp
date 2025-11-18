#include "TitleBall.h"
#include "SpriteRenderer.h"
#include "Easing.h"

constexpr DirectX::XMFLOAT2 ce_fStartBallPos = { -700.0f,700.0f };
constexpr DirectX::XMFLOAT2 ce_fEndBallPos = { 0.0f,150.0f };

CTitleBall::CTitleBall()
	: CAnimationObject()
{
	m_tParam.m_f3Size = DirectX::XMFLOAT3(300.0f,300.0f,0.0f);
}

CTitleBall::~CTitleBall()
{

}

void CTitleBall::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("Ball.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite"));

	m_fDurationTime = 3.5f;
}

void CTitleBall::ExecAnimation()
{
	const float easePosXTime = 3.5f;
	const float easePosYTime = 2.0f;
	const float easeRotateTime = 3.3f;
	
	m_tParam.m_f3Pos.x = ce_fStartBallPos.x + (ce_fEndBallPos.x - ce_fStartBallPos.x) * CEasing::EaseOutBack(m_fTime, easePosXTime);
	m_tParam.m_f3Pos.y = ce_fStartBallPos.y + (ce_fEndBallPos.x - ce_fStartBallPos.y) * CEasing::EaseOutBack(m_fTime, easePosYTime);
	m_tParam.m_f3Rotate.z = 0.0f + (DirectX::XMConvertToRadians(1800.0f) - 0.0f) * CEasing::EaseOutBack(m_fTime, easeRotateTime);
}

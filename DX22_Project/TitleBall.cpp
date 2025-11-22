#include "TitleBall.h"
#include "SpriteRenderer.h"
#include "Easing.h"
#include "Oparation.h"

constexpr DirectX::XMFLOAT3 ce_fStartBallPos = DirectX::XMFLOAT3(-700.0f, 700.0f, 0.0f);
constexpr DirectX::XMFLOAT3 ce_fEndBallPos = DirectX::XMFLOAT3(0.0f,150.0f,0.0f);
constexpr float ce_fEndRotate = DirectX::XMConvertToRadians(1800.0f);

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
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite.cso"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite.cso"));

	m_fDurationTime = 3.5f;
}

void CTitleBall::ExecAnimation()
{
	const float easePosXTime = 3.5f;
	const float easePosYTime = 2.0f;
	const float easeRotateTime = 3.3f;
	
	m_tParam.m_f3Pos = ce_fStartBallPos +  (ce_fEndBallPos - ce_fStartBallPos) * CEasing::EaseOutBack(m_fTime, easePosXTime);
	m_tParam.m_f3Rotate.z = 0.0f + (ce_fEndRotate - 0.0f) * CEasing::EaseOutBack(m_fTime, easeRotateTime);
}

void CTitleBall::EndAnimation()
{
	m_bIsAnimation = false;

	m_tParam.m_f3Pos = ce_fEndBallPos;
	m_tParam.m_f3Rotate.z = ce_fEndRotate;
}

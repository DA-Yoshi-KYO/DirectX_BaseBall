#include "CameraInplay.h"
#include "Ball.h"
#include "Main.h"

CCameraInplay::CCameraInplay()
{
}

CCameraInplay::~CCameraInplay()
{
}

void CCameraInplay::Update()
{
	CBall* pBall = GetScene()->GetGameObject<CBall>();
	DirectX::XMFLOAT3 ballPos = pBall->GetPos();
	m_look.x = ballPos.x;
	m_look.y = 0.0f;
	m_look.z = ballPos.z;

	m_pos.x = ballPos.x;
	m_pos.y = 150.0f;
	m_pos.z = ballPos.z - 100.0f;

	m_up = { 0.0f, 1.0f, 0.0f };
}

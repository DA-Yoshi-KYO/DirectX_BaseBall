#include "Camera.h"
#include "Defines.h"
#include "CameraDebug.h"
#include "CameraEvent.h"
#include "CameraBatter.h"
#include "CameraInplay.h"

std::array<CCamera*, (int)CameraKind::MAX_CAMERA> m_pInstance = {};
CameraKind CCamera::m_eKind = CAM_DEBUG;

CCamera::CCamera()
	: m_pos{ 0.0f + WORLD_AJUST, 10.0f + WORLD_AJUST, 0.0f + WORLD_AJUST }, m_look{ 0.0f + WORLD_AJUST,0.0f + WORLD_AJUST,0.0f + WORLD_AJUST }, m_up{ 0.0f,1.0f,0.0f }
	, m_fovy(DirectX::XMConvertToRadians(60.0f)), m_aspect(16.0f / 9.0f)
	, m_near(CMETER(30.0f)), m_far(METER(1000.0f))
{

}

CCamera::~CCamera()
{

}

DirectX::XMFLOAT4X4 CCamera::GetViewMatrix(bool transpose)
{
	DirectX::XMFLOAT4X4 mat;
	DirectX::XMMATRIX view;

	view = DirectX::XMMatrixLookAtLH(
		DirectX::XMVectorSet(m_pos.x, m_pos.y, m_pos.z, 0.0f),
		DirectX::XMVectorSet(m_look.x, m_look.y, m_look.z, 0.0f),
		DirectX::XMVectorSet(m_up.x, m_up.y, m_up.z, 0.0f));

	if (transpose) view = DirectX::XMMatrixTranspose(view);

	DirectX::XMStoreFloat4x4(&mat, view);

	return mat;
}

DirectX::XMFLOAT4X4 CCamera::GetProjectionMatrix(bool transpose)
{
	DirectX::XMFLOAT4X4 mat;
	DirectX::XMMATRIX proj;
	proj = DirectX::XMMatrixPerspectiveFovLH(m_fovy, m_aspect, m_near, m_far);

	if (transpose)proj = DirectX::XMMatrixTranspose(proj);

	DirectX::XMStoreFloat4x4(&mat, proj);

	return mat;
}

CCamera* CCamera::GetInstance()
{
	if (m_pInstance[(int)m_eKind])
	{
		switch (m_eKind)
		{
		case CAM_DEBUG: m_pInstance[(int)m_eKind] = new CCameraDebug(); break;
		case CAM_EVENT: m_pInstance[(int)m_eKind] = new CCameraEvent(); break;
		case CAM_BATTER: m_pInstance[(int)m_eKind] = new CCameraBatter(); break;
		case CAM_INPLAY: m_pInstance[(int)m_eKind] = new CCameraInplay(); break;
		}
	}

	return m_pInstance[(int)m_eKind];
}

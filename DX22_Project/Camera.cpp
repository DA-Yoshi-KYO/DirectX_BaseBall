#include "Camera.h"
#include "Defines.h"

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
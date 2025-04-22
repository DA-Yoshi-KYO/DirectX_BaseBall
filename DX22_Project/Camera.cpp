#include "Camera.h"
#include "Defines.h"
#include "CameraDebug.h"
#include "CameraEvent.h"
#include "CameraMinimap.h"
#include "CameraBatter.h"
#include "CameraInplay.h"

CameraKind CCamera::m_eCameraKind = CameraKind::CAM_DEBUG;

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

const DirectX::XMFLOAT4X4 CCamera::Get2DWolrdMatrix(DirectX::XMFLOAT2 pos, float rotate, bool transpose)
{
	DirectX::XMMATRIX mWorld = 
		DirectX::XMMatrixScaling(1.0f, -1.0f, 1.0f) *
		DirectX::XMMatrixRotationZ(rotate) *
		DirectX::XMMatrixTranslation(SCREEN_WIDTH / 2.0f + pos.x, SCREEN_HEIGHT / 2.0f - pos.y, 0.0f);

	if (transpose) mWorld = DirectX::XMMatrixTranspose(mWorld);

	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, mWorld);

	return world;
}

const DirectX::XMFLOAT4X4 CCamera::Get2DViewMatrix(bool transpose)
{
	DirectX::XMMATRIX mView = DirectX::XMMatrixLookAtLH(
		DirectX::XMVectorSet(0.0f, 0.0f, -0.02f, 0.0f),
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

	if (transpose) mView = DirectX::XMMatrixTranspose(mView);

	DirectX::XMFLOAT4X4 view;
	DirectX::XMStoreFloat4x4(&view, mView);

	return view;
}

const DirectX::XMFLOAT4X4 CCamera::Get2DProjectionMatrix(bool transpose)
{
	DirectX::XMMATRIX mProj = 
		DirectX::XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.1f, 10.0f);

	if (transpose) mProj = DirectX::XMMatrixTranspose(mProj);

	DirectX::XMFLOAT4X4 proj;
	DirectX::XMStoreFloat4x4(&proj, mProj);

	return proj;
}

std::unique_ptr<CCamera>& CCamera::GetInstance(int CamKind)
{
	static std::unique_ptr<CCamera> CamInstance[] = {
		std::make_unique<CCameraDebug>(),
		std::make_unique<CCameraEvent>(),
		//std::make_unique<CCameraMinimap>(),
		std::make_unique<CCameraBatter>(),
		std::make_unique<CCameraInplay>(),
	};

	switch (CamKind)
	{
	case CAM_DEBUG:		return CamInstance[CAM_DEBUG];		break;
	case CAM_EVENT :	return CamInstance[CAM_EVENT];		break;
	//case CAM_MINIMAP :	return CamInstance[CAM_MINIMAP];	break;
	case CAM_BATTER :	return CamInstance[CAM_BATTER];		break;
	case CAM_INPLAY :	return CamInstance[CAM_INPLAY];		break;
	default: return CamInstance[CAM_DEBUG]; break;
	}
}

void CCamera::SetCameraKind(CameraKind kind)
{
	m_eCameraKind = kind;
}

CameraKind CCamera::GetCameraKind()
{
	return m_eCameraKind;
}

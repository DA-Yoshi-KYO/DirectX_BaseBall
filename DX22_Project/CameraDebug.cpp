#include "CameraDebug.h"
#include "Defines.h"

CCameraDebug::CCameraDebug()
	: m_radXZ(0.0f), m_radY(DirectX::XMConvertToRadians(00.0f)), m_radius(0.0f)
{
	m_pos = { WORLD_AJUST,WORLD_AJUST, WORLD_AJUST + 161.0f };
	m_look = { WORLD_AJUST,WORLD_AJUST + 3.5f, WORLD_AJUST + 111.0f };
}

CCameraDebug::~CCameraDebug()
{
}

void CCameraDebug::Update()
{
	if (IsKeyPress(VK_UP))m_look.z -= CAMERA_SPEED;
	if (IsKeyPress(VK_DOWN))m_look.z += CAMERA_SPEED;
	if (IsKeyPress(VK_RIGHT))m_look.x -= CAMERA_SPEED;
	if (IsKeyPress(VK_LEFT))m_look.x += CAMERA_SPEED;
	if (IsKeyPress(VK_SHIFT))m_look.y += CAMERA_SPEED;
	if (IsKeyPress(VK_CONTROL))m_look.y -= CAMERA_SPEED;

	if (IsKeyPress('A')) m_radXZ += CAMERA_ROTATE;
	if (IsKeyPress('D')) m_radXZ -= CAMERA_ROTATE;
	if (IsKeyPress('W')) m_radY += CAMERA_ROTATE;
	if (IsKeyPress('S')) m_radY -= CAMERA_ROTATE;

	if (IsKeyPress('E')) m_radius += CAMERA_SPEED;
	if (IsKeyPress('Q')) m_radius -= CAMERA_SPEED;

	if (m_radius <= 10.0f) m_radius = 10.0f;

	m_pos.x = cosf(m_radY) * sinf(m_radXZ) * m_radius + m_look.x;
	m_pos.y = sinf(m_radY) * m_radius + m_look.y;
	m_pos.z = cosf(m_radY) * cosf(m_radXZ) * m_radius + m_look.z;
}

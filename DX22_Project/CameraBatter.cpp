#include "CameraBatter.h"
#include "Defines.h"
#include "Input.h"

#define CAMERA_SPEEDQ (0.5f)
#define CAMERA_ROTATEQ (0.01f)

CCameraBatter::CCameraBatter()
	: m_radXZ(0.0f), m_radY(DirectX::XMConvertToRadians(30.0f)), m_radius(0.0f)
{
	m_pos = { WORLD_AJUST,WORLD_AJUST + 10.0f, WORLD_AJUST + 170.0f };
	m_look = { WORLD_AJUST,WORLD_AJUST + 0.0f, WORLD_AJUST - 10.0f };
}

CCameraBatter::~CCameraBatter()
{
}

void CCameraBatter::Update()
{
	//if (IsKeyPress(VK_UP))m_look.z -= CAMERA_SPEEDQ;
	//if (IsKeyPress(VK_DOWN))m_look.z += CAMERA_SPEEDQ;
	//if (IsKeyPress(VK_RIGHT))m_look.x -= CAMERA_SPEEDQ;
	//if (IsKeyPress(VK_LEFT))m_look.x += CAMERA_SPEEDQ;
	//if (IsKeyPress(VK_SHIFT))m_look.y += CAMERA_SPEEDQ;
	//if (IsKeyPress(VK_CONTROL))m_look.y -= CAMERA_SPEEDQ;

	//if (IsKeyPress('A')) m_radXZ += CAMERA_ROTATEQ;
	//if (IsKeyPress('D')) m_radXZ -= CAMERA_ROTATEQ;
	//if (IsKeyPress('W')) m_radY += CAMERA_ROTATEQ;
	//if (IsKeyPress('S')) m_radY -= CAMERA_ROTATEQ;

	//if (IsKeyPress('E')) m_radius += CAMERA_SPEEDQ;
	//if (IsKeyPress('Q')) m_radius -= CAMERA_SPEEDQ;

	//if (m_radius <= 10.0f) m_radius = 10.0f;

	//m_pos.x = cosf(m_radY) * sinf(m_radXZ) * m_radius + m_look.x;
	//m_pos.y = sinf(m_radY) * m_radius + m_look.y;
	//m_pos.z = cosf(m_radY) * cosf(m_radXZ) * m_radius + m_look.z;


}

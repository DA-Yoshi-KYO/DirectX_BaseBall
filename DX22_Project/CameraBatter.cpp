#include "CameraBatter.h"
#include "Defines.h"
#include "Input.h"

#define CAMERA_SPEEDQ (0.5f)
#define CAMERA_ROTATEQ (0.01f)

CCameraBatter::CCameraBatter()
	: m_radXZ(0.0f), m_radY(DirectX::XMConvertToRadians(30.0f)), m_radius(0.0f)
{
	m_pos = { WORLD_AJUST,WORLD_AJUST, WORLD_AJUST + 161.0f };
	m_look = { WORLD_AJUST,WORLD_AJUST + 3.5f, WORLD_AJUST + 111.0f };
}

CCameraBatter::~CCameraBatter()
{
}

void CCameraBatter::Update()
{

}

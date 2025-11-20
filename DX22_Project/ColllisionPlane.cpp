#include "ColllisionPlane.h"

void ColllisionPlane::Init()
{
	m_tCollisionInfo.type = Collision::ePlane;
}

void ColllisionPlane::SetInfo(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 normal)
{
	m_tCollisionInfo.plane.pos = pos;
	m_tCollisionInfo.plane.normal = normal;
}
#include "CollisionTriangle.h"

void CCollisionTriangle::Init()
{
	m_tCollisionInfo.type = Collision::eTriangle;
}

void CCollisionTriangle::SetInfo(DirectX::XMFLOAT3* point)
{
	m_tCollisionInfo.triangle.point[0] = point[0];
	m_tCollisionInfo.triangle.point[1] = point[1];
	m_tCollisionInfo.triangle.point[2] = point[2];
}

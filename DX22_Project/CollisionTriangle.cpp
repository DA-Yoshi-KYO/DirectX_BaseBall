#include "CollisionTriangle.h"

void CCollisionTriangle::Init()
{
	m_tCollisionInfo.type = Collision::eTriangle;
}

void CCollisionTriangle::SetInfo(DirectX::XMFLOAT3 point0, DirectX::XMFLOAT3 point1, DirectX::XMFLOAT3 point2)
{
	m_tCollisionInfo.triangle.point[0] = point0;
	m_tCollisionInfo.triangle.point[1] = point1;
	m_tCollisionInfo.triangle.point[2] = point2;
}

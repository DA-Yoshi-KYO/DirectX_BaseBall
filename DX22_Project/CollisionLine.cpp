#include "CollisionLine.h"

void CollisionLine::Init()
{
	m_tCollisionInfo.type = Collision::eLine;
}

void CollisionLine::SetInfo(DirectX::XMFLOAT3 origin, DirectX::XMFLOAT3 end)
{
	m_tCollisionInfo.line.start = origin;
	m_tCollisionInfo.line.end = end;
}
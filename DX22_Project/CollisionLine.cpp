#include "CollisionLine.h"

void CCollisionLine::Init()
{
	m_tCollisionInfo.type = Collision::eLine;
}

void CCollisionLine::SetInfo(DirectX::XMFLOAT3 origin, DirectX::XMFLOAT3 end)
{
	m_tCollisionInfo.line.start = origin;
	m_tCollisionInfo.line.end = end;
}
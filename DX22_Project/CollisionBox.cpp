#include "CollisionBox.h"
#include "CollisionLine.h"

void CCollisionBox::Init()
{
	m_tCollisionInfo.type = Collision::eBox;
}

void CCollisionBox::SetInfo(DirectX::XMFLOAT3 center, DirectX::XMFLOAT3 size)
{
	m_tCollisionInfo.box.center = center;
	m_tCollisionInfo.box.size = size;
}

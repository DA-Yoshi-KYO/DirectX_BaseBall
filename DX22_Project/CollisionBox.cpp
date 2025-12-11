#include "CollisionBox.h"

void CCollisionBox::Init()
{
    m_tCollisionInfo.type = Collision::eBox;
}

void CCollisionBox::SetInfo(DirectX::XMFLOAT3 center, DirectX::XMFLOAT3 size)
{
    m_tCollisionInfo.box.center = center;
    m_tCollisionInfo.box.size = size;
}

void CCollisionBox::SetInfo(Collision::Box box)
{
    m_tCollisionInfo.box = box;
}

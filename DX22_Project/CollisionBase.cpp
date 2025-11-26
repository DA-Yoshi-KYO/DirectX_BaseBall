// インクルード部
#include "CollisionBase.h"

CCollisionBase::~CCollisionBase()
{

}

void CCollisionBase::Draw()
{
    Collision::DrawCollision(m_tCollisionInfo);
}

Collision::Result CCollisionBase::IsHit(CCollisionBase* other)
{
    Collision::Result result = Collision::Hit(m_tCollisionInfo, other->GetInfo());

    return result;
}

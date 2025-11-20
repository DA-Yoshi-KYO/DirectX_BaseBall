#pragma once

#include "CollisionBase.h"

class CollisionTriangle : public CCollisionBase
{
public:
	using CCollisionBase::CCollisionBase;
	void Init() override;
	void SetInfo(DirectX::XMFLOAT3* point);
};


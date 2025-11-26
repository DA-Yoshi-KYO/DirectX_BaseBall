#pragma once

#include "CollisionBase.h"

class CCollisionTriangle : public CCollisionBase
{
public:
	using CCollisionBase::CCollisionBase;
	void Init() override;
	void SetInfo(DirectX::XMFLOAT3 point0, DirectX::XMFLOAT3 point1, DirectX::XMFLOAT3 point2);
};


#pragma once

#include "CollisionBase.h"

class CollisionLine :  public CCollisionBase
{
public:
	using CCollisionBase::CCollisionBase;
	void Init() override;
	void SetInfo(DirectX::XMFLOAT3 origin, DirectX::XMFLOAT3 end);
};


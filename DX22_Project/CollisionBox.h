#pragma once

#include "CollisionBase.h"

class CollisionBox : public CCollisionBase
{
public:
	using CCollisionBase::CCollisionBase;
	void Init() override;
	void SetInfo(DirectX::XMFLOAT3 center, DirectX::XMFLOAT3 size);
	
};

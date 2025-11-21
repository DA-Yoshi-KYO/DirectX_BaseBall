#pragma once

#include "CollisionBase.h"

class CCollisionBox : public CCollisionBase
{
public:
	using CCollisionBase::CCollisionBase;
	void Init() override;
	void SetInfo(DirectX::XMFLOAT3 center, DirectX::XMFLOAT3 size);
	
};

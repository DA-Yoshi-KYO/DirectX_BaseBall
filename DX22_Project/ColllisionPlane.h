#pragma once

#include "CollisionBase.h"

class ColllisionPlane : public CCollisionBase
{
public:
	using CCollisionBase::CCollisionBase;
	void Init() override;
	void SetInfo(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 normal);
};


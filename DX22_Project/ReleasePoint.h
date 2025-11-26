#pragma once

#include "GameObject.h"

class CReleasePoint : public CGameObject
{
public:
	CReleasePoint();
	~CReleasePoint();
	void Init() override;
	void Update() override;

	void SetFuturePos(DirectX::XMFLOAT3 pos) { m_f3Future = pos; }

private:
	DirectX::XMFLOAT3 m_f3Future;
};


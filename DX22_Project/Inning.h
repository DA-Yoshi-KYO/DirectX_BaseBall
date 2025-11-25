#pragma once

#include "GameObject.h"

class CInning : public CGameObject
{
public:
	CInning();
	~CInning();
	void Init() override;
	void Draw() override;
	void InningProgress();
	int Inspecter(bool isEnd = true) override;

private:
	int m_nInningNum;
	bool m_bIsStart;
	DirectX::XMFLOAT3 m_f3InningPos;
	DirectX::XMFLOAT3 m_f3TopBottomPos;

};


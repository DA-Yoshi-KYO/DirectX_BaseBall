#pragma once
#include "GameObject.h"

class CCountObject : public CGameObject
{
public:
	CCountObject();
	virtual ~CCountObject();
	void Init() override;
	void SetActive(bool isActive) { m_bActive = isActive; }
	bool GetActive() { return m_bActive; }

protected:
	bool m_bActive;
};


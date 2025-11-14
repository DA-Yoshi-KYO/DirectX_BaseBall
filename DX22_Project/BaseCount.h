#pragma once
#include "CountObject.h"

class CBaseCount : public CCountObject
{
public:
	CBaseCount();
	~CBaseCount();
	void Update() override;
};

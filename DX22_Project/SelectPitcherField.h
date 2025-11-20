#pragma once

#include "MemberSelectFieldBase.h"

class CSelectPitcherField : public CMemberSelectFieldBase
{
public:
	CSelectPitcherField();
	~CSelectPitcherField();
	void Update() override;
};


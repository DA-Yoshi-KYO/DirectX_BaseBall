#pragma once

#include "MemberSelectFieldBase.h"

class CStartingLineupField : public CMemberSelectFieldBase
{
public:
	CStartingLineupField();
	~CStartingLineupField();
	void Update() override;

};

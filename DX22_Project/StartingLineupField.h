#pragma once

#include "MemberSelectFieldBase.h"
#include "FielderData.h"
#include "PitcherData.h"

class CStartingLineupField : public CMemberSelectFieldBase
{
public:
	CStartingLineupField();
	~CStartingLineupField();
	void Init(int playerNo, std::list<CFielderData*> list, CPitcherData* starter);
	void Update() override;

};

#pragma once

#include "MemberSelectFieldBase.h"
#include "FielderData.h"

class CSelectFielderField : public CMemberSelectFieldBase
{
public:
	CSelectFielderField();
	~CSelectFielderField();
	void Update()override;
	void Init(int playerNo, std::list<CFielderData*> list);

};


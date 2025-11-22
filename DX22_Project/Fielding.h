#pragma once

#include "Fielder.h"
#include "FielderData.h"

class CFielding
{
public:
	CFielding();
	~CFielding();
	void Init();
	void Update(int DefencePlayer);

private:
	CFielder* m_pFielder[(int)Positions::Max];

};
#pragma once

#include "Fielder.h"
#include <array>

class CFielding
{
public:
	CFielding();
	~CFielding();
	void Init();
	void Update(int DefencePlayer);
	void SetFielderData(int DefencePlayer);

private:
	std::array<CFielder*, (int)Positions::Max> m_pFielder;

};
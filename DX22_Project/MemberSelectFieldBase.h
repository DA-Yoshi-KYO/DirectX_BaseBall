#pragma once

#include "PlayerDataBase.h"
#include "MemberIcon.h"
#include <vector>

class CMemberSelectFieldBase
{
public:
	CMemberSelectFieldBase();
	virtual ~CMemberSelectFieldBase();
	void Init(int playerNo, std::list<CPlayerDataBase*> list);
	virtual void Update() = 0;

protected:
	std::vector<std::unique_ptr<CMemberIcon>> m_pMemberList;
	int m_nPlayerNo;

};

#pragma once

#include "MemberIcon.h"
#include <vector>

class CMemberSelectFieldBase
{
public:
	CMemberSelectFieldBase();
	~CMemberSelectFieldBase();
	void Init(int playerNo);
	virtual void Update() = 0;
	void SetMemberList(std::vector<CMemberIcon*> list) { m_pMemberList = list; }

protected:
	std::vector<CMemberIcon*> m_pMemberList;
	int m_nPlayerNo;

};

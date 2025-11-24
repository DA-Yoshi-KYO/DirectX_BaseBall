#include "MemberSelectFieldBase.h"

CMemberSelectFieldBase::CMemberSelectFieldBase()
	: m_pMemberList{}, m_nPlayerNo(1)
	, m_bActive(false)
{

}

CMemberSelectFieldBase::~CMemberSelectFieldBase()
{

}

void CMemberSelectFieldBase::Init(int playerNo, std::list<CPlayerDataBase*> list)
{
	m_nPlayerNo = playerNo - 1;
	
	for (auto itr : list)
	{
		auto& data = m_pMemberList.emplace_back(std::make_unique<CMemberIcon>());
		data->Init(itr);
	}
}

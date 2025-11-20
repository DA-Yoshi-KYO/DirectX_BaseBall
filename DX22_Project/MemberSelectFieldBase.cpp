#include "MemberSelectFieldBase.h"

CMemberSelectFieldBase::CMemberSelectFieldBase()
	: m_pMemberList{}, m_nPlayerNo(1)
{

}

CMemberSelectFieldBase::~CMemberSelectFieldBase()
{

}

void CMemberSelectFieldBase::Init(int playerNo)
{
	m_nPlayerNo = playerNo - 1;
}

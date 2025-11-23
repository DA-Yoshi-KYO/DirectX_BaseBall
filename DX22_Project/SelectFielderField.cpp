#include "SelectFielderField.h"

CSelectFielderField::CSelectFielderField()
	: CMemberSelectFieldBase()
{

}

CSelectFielderField::~CSelectFielderField()
{

}

void CSelectFielderField::Update()
{

}

void CSelectFielderField::Init(int playerNo, std::list<CFielderData*> list)
{
	m_nPlayerNo = playerNo - 1;

	for (auto itr : list)
	{
		auto& data = m_pMemberList.emplace_back(std::make_unique<CMemberIcon>());
		data->Init(itr);
	}
}

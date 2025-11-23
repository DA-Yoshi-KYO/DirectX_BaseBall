#include "SelectPitcherField.h"

CSelectPitcherField::CSelectPitcherField()
	: CMemberSelectFieldBase()
{

}

CSelectPitcherField::~CSelectPitcherField()
{

}

void CSelectPitcherField::Update()
{

}

void CSelectPitcherField::Init(int playerNo, std::list<CPitcherData*> list)
{
	m_nPlayerNo = playerNo - 1;

	for (auto itr : list)
	{
		auto& data = m_pMemberList.emplace_back(std::make_unique<CMemberIcon>());
		data->Init(itr);
	}
}

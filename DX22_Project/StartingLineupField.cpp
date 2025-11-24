#include "StartingLineupField.h"

CStartingLineupField::CStartingLineupField()
	: CMemberSelectFieldBase()
{

}

CStartingLineupField::~CStartingLineupField()
{

}

void CStartingLineupField::Init(int playerNo, std::list<CFielderData*> list, CPitcherData* starter)
{
	m_nPlayerNo = playerNo - 1;

	for (auto itr : list)
	{
		auto& data = m_pMemberList.emplace_back(std::make_unique<CMemberIcon>());
		data->Init(itr);
	}
	auto& data = m_pMemberList.emplace_back(std::make_unique<CMemberIcon>());
	data->Init(starter);
}

void CStartingLineupField::Update()
{
	for (int i = 0; i < m_pMemberList.size(); i++)
	{
		m_pMemberList[i]->SetPos(DirectX::XMFLOAT3(310.0f + (SCREEN_WIDTH * 0.5f + 125.0f) * m_nPlayerNo, 205.0f + 47.0f * i, 0.0f));
	}
}

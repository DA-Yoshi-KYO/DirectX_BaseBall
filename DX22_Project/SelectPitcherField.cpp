#include "SelectPitcherField.h"
#include "Main.h"

CSelectPitcherField::CSelectPitcherField()
	: CMemberSelectFieldBase()
{

}

CSelectPitcherField::~CSelectPitcherField()
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

	m_pPitcherBack = GetScene()->AddGameObject<CBenchPitcherBack>("BenchPitcherBack",Tag::UI);
	m_pPitcherBack->Init(m_nPlayerNo);
	m_pPitcherBack->SetSize(DirectX::XMFLOAT3(1280.0f, 720.0f, 0.0f));
}

void CSelectPitcherField::Update()
{
	m_pPitcherBack->SetSelectable(m_bActive);
}

#include "SelectFielderField.h"
#include "Main.h"

CSelectFielderField::CSelectFielderField()
	: CMemberSelectFieldBase()
{

}

CSelectFielderField::~CSelectFielderField()
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
	m_pBatterBack = GetScene()->AddGameObject<CBenchBatterBack>("BenchBatterBack", Tag::UI);
	m_pBatterBack->Init(m_nPlayerNo);
	m_pBatterBack->SetSize(DirectX::XMFLOAT3(1280.0f, 720.0f, 0.0f));
}

void CSelectFielderField::Update()
{
	m_pBatterBack->SetSelectable(m_bActive);
}

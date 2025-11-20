#include "StartingLineupField.h"

CStartingLineupField::CStartingLineupField()
	: CMemberSelectFieldBase()
{

}

CStartingLineupField::~CStartingLineupField()
{

}

void CStartingLineupField::Update()
{
	for (int i = 0; i < m_pMemberList.size(); i++)
	{
		m_pMemberList[i]->SetPos(DirectX::XMFLOAT3(300.0f + 800.0f * m_nPlayerNo, -300.0f + 100.0f * i, 0.0f));
	}
}
#pragma once

#include "MemberBack.h"
#include "MemberName.h"
#include "PlayerDataBase.h"

class CMemberIcon
{
public:
	CMemberIcon();
	~CMemberIcon();
	void Init(CPlayerDataBase* pData);
	CPlayerDataBase* GetOwner() { return m_pPlayerData; }
	void SetPos(DirectX::XMFLOAT3 pos);
	
private:
	CMemberBack* m_pBack;
	CMemberName* m_pName;
	CPlayerDataBase* m_pPlayerData;

};

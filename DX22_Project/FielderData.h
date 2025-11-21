#pragma once

#include "PlayerDataBase.h"

struct FielderData
{
	int m_nTrajectory;
	Quality m_eMeet;
	Quality m_ePower;
	Quality m_eSpeed;
	Quality m_eThrowing;
	Quality m_eDefence;
	Quality m_eChatch;
};

class CFielderData : public CPlayerDataBase
{
public:
	CFielderData();
	~CFielderData();
	void SetFielderData(FielderData data) { m_tFielderData = data; }
	FielderData GetFielderData() { return m_tFielderData; }

protected:
	FielderData m_tFielderData;

};

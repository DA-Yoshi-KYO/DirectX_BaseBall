#pragma once

#include "PlayerData.h"

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

class CFielder : public CPlayerDataBase
{
public:
	CFielder();
	~CFielder();
	void SetFielderData(FielderData data) { m_tFielderData = data; }
	FielderData SetFielderData() { return m_tFielderData; }

protected:
	FielderData m_tFielderData;

};

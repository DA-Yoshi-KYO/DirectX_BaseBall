#pragma once
#include "PlayerDataBase.h"

enum PitcherKind
{
	Starter,
	Relief
};

struct PitcherData
{
	float m_fSpeed;
	Quality m_eStamina;
	Quality m_eControl;
	PitcherKind m_eKind;
};

class CPitcherData : public CPlayerDataBase
{
public:
	CPitcherData();
	~CPitcherData();
	void SetPitcherData(PitcherData data) { m_tPitcherData = data; }
	PitcherData GetPitcherData() { return m_tPitcherData; }

private:
	PitcherData m_tPitcherData;

};

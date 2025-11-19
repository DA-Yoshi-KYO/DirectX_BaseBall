#pragma once
#include "PlayerData.h"

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

class CPitcher : public CPlayerDataBase
{
public:
	CPitcher();
	~CPitcher();
	void SetPitcherData(PitcherData data) { m_tPitcherData = data; }
	PitcherData GetPitcherData() { return m_tPitcherData; }

private:
	PitcherData m_tPitcherData;

};

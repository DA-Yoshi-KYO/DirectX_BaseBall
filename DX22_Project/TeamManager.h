#pragma once
#include <vector>
#include <memory>
#include "Defines.h"

class CTeamManager
{
private:

public:
	enum Teams
	{
		Bears,
		Rabbits,

		Max
	};

	enum Quality
	{
		G,
		F,
		E,
		D,
		C,
		B,
		A,
		S
	};
public:
	CTeamManager();
	virtual ~CTeamManager();
	virtual void Load() = 0;

	
private:
	struct PitcherState
	{
		float m_fSpeed;
		float m_fStamina;
		float m_fControl;
		bool m_bLefty;
		bool m_bEntry;
		bool m_bLeave;
	};
	
	struct BatterState
	{
		int Trajectory;
		float m_fMeat;
		float m_fPower;
		float m_fSpeed;
		float m_fThrowing;
		float m_fDeiffence;
		float m_fChatch;
		bool m_bLefty;
		bool m_bEntry;
		bool m_bLeave;
	};

protected:
	std::vector<PitcherState> m_tVecPitcherMember;
	std::vector<BatterState> m_tVecBatterMember;
	std::vector<BatterState> m_tVecLineup;

};
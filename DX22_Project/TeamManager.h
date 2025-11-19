#pragma once

#include "PlayerData.h"
#include "Team.h"
#include <array>

class CTeamDirector
{
public:
	CTeamDirector(int PlayerNo);
	~CTeamDirector();
	void TeamInit();
	void TeamUninit();	// 明示的な終了処理(破棄タイミングはゲーム終了後)
	CTeam* GetTeam();

private:
	static std::array<CTeam*,2> m_pTeam;
	int m_nPlayerNo;

private:
	void InitStarter();

};

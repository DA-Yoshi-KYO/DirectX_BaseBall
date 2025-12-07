#pragma once

#include "PlayerDataBase.h"
#include "Team.h"
#include "GameDirecterBase.h"
#include <array>

class CTeamDirector : public CGameDirecterBase
{
public:
	CTeamDirector(int PlayerNo);
	~CTeamDirector();
	void TeamInit();
	static void TeamUninit();	// 明示的な終了処理(破棄タイミングはゲーム終了後)
	CTeam* GetTeam();

	void EndInplay() override;

private:
	static std::array<CTeam*,2> m_pTeam;
	int m_nPlayerNo;

private:
	void InitStarter();
	void InitStartingLineup();

};

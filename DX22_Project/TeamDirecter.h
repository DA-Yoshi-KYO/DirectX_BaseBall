#pragma once

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
    int GetTeamNo() { return m_nPlayerIndex + 1; }

    void EndInplay(bool isAttack);

private:
    static std::array<CTeam*,2> m_pTeam;
    int m_nPlayerIndex;

private:
    void InitStarter();
    void InitStartingLineup();

};

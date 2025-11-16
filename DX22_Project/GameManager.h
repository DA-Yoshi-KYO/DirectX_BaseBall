#pragma once

#include "CountManager.h"
#include "AttackManager.h"
#include "DefenceManager.h"
#include "TeamManager.h"

#define PLAYER_1 1
#define PLAYER_2 2

enum class GamePhase
{
    Batting,
    InPlay
};

class CGameManager
{
private:
    CGameManager();
    
public:
    ~CGameManager();
    void Init();
    void Update();
    void Draw();
    GamePhase GetPhase() { return m_ePhase; }

public:
    static CGameManager* GetInstance();
    CCountManager* GetCountManager() { return m_pCountManager.get(); };
    CAttackManager* GetAttackManager() { return m_pAttackManager.get(); };
    CDefenceManager* GetDefenceManager() { return m_pDefenceManager.get(); };
    CTeamManager* GetTeamManager(int teamNo) { return m_pTeamManager[teamNo].get(); };

private:
    static CGameManager* m_pInstance;
    std::unique_ptr<CCountManager> m_pCountManager;
    std::unique_ptr<CAttackManager> m_pAttackManager;
    std::unique_ptr<CDefenceManager> m_pDefenceManager;
    std::unique_ptr<CTeamManager> m_pTeamManager[2];
    GamePhase m_ePhase;

};
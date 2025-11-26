#pragma once

#include "CountManager.h"
#include "AttackManager.h"
#include "DefenceManager.h"
#include "TeamManager.h"
#include "FieldManager.h"

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
    void SetPhase(GamePhase phase) { m_ePhase = phase; }

public:
    static CGameManager* GetInstance();
    void Release();
    CCountManager* GetCountManager() { return m_pCountManager.get(); };
    CAttackManager* GetAttackManager() { return m_pAttackManager.get(); };
    CDefenceManager* GetDefenceManager() { return m_pDefenceManager.get(); };
    CTeamDirector* GetTeamManager(int teamNo) { return m_pTeamManager[teamNo - 1].get(); };

private:
    static CGameManager* m_pInstance;
    std::unique_ptr<CCountManager> m_pCountManager;
    std::unique_ptr<CAttackManager> m_pAttackManager;
    std::unique_ptr<CDefenceManager> m_pDefenceManager;
    std::unique_ptr<CFieldManager> m_pFieldManager;
    std::unique_ptr<CTeamDirector> m_pTeamManager[2];
    GamePhase m_ePhase;

};
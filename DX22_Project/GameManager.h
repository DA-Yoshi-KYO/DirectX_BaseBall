#pragma once

#include "CountDirecter.h"
#include "AttackDirecter.h"
#include "DefenceDirecter.h"
#include "TeamDirecter.h"
#include "FieldDirecter.h"

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
    void EndAllInplay();
    void ChangeInning();
    void FaulBall();
    void HomeRun();

public:
    static CGameManager* GetInstance();
    void Release();
    CCountDirecter* GetCountDirecter() { return m_pCountDirecter.get(); };
    CAttackDirecter* GetAttackDirecter() { return m_pAttackDirecter.get(); };
    CDefenceDirecter* GetDefenceDirecter() { return m_pDefenceDirecter.get(); };
    CTeamDirector* GetTeamDirecter(int teamNo) { return m_pTeamDirecter[teamNo - 1].get(); };

private:
    static CGameManager* m_pInstance;
    std::unique_ptr<CCountDirecter> m_pCountDirecter;
    std::unique_ptr<CAttackDirecter> m_pAttackDirecter;
    std::unique_ptr<CDefenceDirecter> m_pDefenceDirecter;
    std::unique_ptr<CFieldDirecter> m_pFieldDirecter;
    std::unique_ptr<CTeamDirector> m_pTeamDirecter[2];
    GamePhase m_ePhase;
    float m_fWaitTime;

    void CheckEndInplay();
};
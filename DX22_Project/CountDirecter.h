#pragma once

#include <array>
#include "StrikeCount.h"
#include "BallCount.h"
#include "OutCount.h"
#include "BaseCount.h"
#include "Score.h"
#include "Inning.h"
#include "ScoreBoard.h"
#include "GameDirecterBase.h"

class CCountDirecter : public CGameDirecterBase
{
public:
    CCountDirecter();
    ~CCountDirecter();
    void Init()override;
    void Update();
    void EndInplay() override;

    void AddStrikeCount();
    void AddBallCount();
    void AddOutCount();
    void SetIsBase(bool isBase, int index);

private:
    std::array<CStrikeCount*, 2> m_pStrikeCount;
    std::array<CBallCount*, 3> m_pBallCount;
    std::array<COutCount*, 2> m_pOutCount;
    std::array<CBaseCount*, 3> m_pBaseCount;
    std::array<CScore*, 2> m_pScore;
    CInning* m_pInning;
    CScoreBoard* m_pScoreBoard;

    struct CountParam
    {
        int m_nStrikeCount;
        int m_nBallCount;
        int m_nOutCount;
    }m_tParam;

private:
    void ThreeStrike();
    void FourBall();
    void ThreeOut();

};


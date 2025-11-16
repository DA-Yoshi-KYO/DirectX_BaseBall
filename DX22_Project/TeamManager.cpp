#include "TeamManager.h"

void CTeamManager::Init(int teamNo)
{
    m_nPlayerNo = teamNo;
    m_eTeamKind = Teams::Monkeys;

    m_tVecBatterMember.clear();
    m_tVecPitcherMember.clear();
    m_nTakingNo = 1;
}

CTeamManager::CTeamManager()
    : m_eTeamKind(Teams::Monkeys)
    , m_tVecBatterMember{}, m_tVecPitcherMember{}
    , m_nTakingNo(1)
{

}

CTeamManager::~CTeamManager()
{

}

bool CTeamManager::Load(Teams team)
{
    const std::string sPath[] =
    {
        "Bears.csv",
        "Rabbits.csv",
        "Tigers.csv",
        "Elephants.csv",
        "Monkeys.csv",
    };
    std::wstring wsPath[] =
    {
        L"Bears.csv",
        L"Rabbits.csv",
        L"Tigers.csv",
        L"Elephants.csv",
        L"Monkeys.csv",
    };
    std::wifstream fFile(PATH_DATA(wsPath[(int)team]));

    if (!fFile)
    {
        ERROR_MESSAGE(sPath[(int)team].c_str());
        Init();
        return false;
    }

    std::locale jpLocale("Japanese_Japan.932");
    fFile.imbue(jpLocale);

    std::wstring line;
    std::vector<std::wstring> row;
    std::vector<std::vector<std::wstring>> rows;

    m_tVecPitcherMember.clear();
    m_tVecBatterMember.clear();

    while (std::getline(fFile, line))
    {
        std::wstringstream ss(line);
        std::wstring cell;
        std::vector<std::wstring> row;

        while (std::getline(ss, cell, L','))
        {
            row.push_back(cell);
        }

        rows.push_back(row);
    }

    for (int i = 0; i < rows.size(); i++)
    {
        for (int j = 0; j < rows[i].size(); j++)
        {
            OutputDebugStringW(rows[i][j].c_str());
        }
        OutputDebugStringA("\n");
    }

    fFile.close();

    int nPitcherBenchNo = 0;
    int nBatterBenchNo = 0;

    for (int i = 1; i < rows.size(); i++)
    {
        // “ŠŽè‚©–ìŽè‚©”»’è
        if (rows[i][csvData::PitcherBatter] == L"“ŠŽè")
        {
            PitcherState tState;
            tState.m_sName = rows[i][csvData::Name];
            tState.m_bLefty = rows[i][csvData::Handed] == L"¶";
            tState.m_fSpeed = std::stof(rows[i][csvData::BallSpeed]);
            tState.m_eStamina = (Quality)std::stoi(rows[i][csvData::Stamina]);
            tState.m_eControl = (Quality)std::stoi(rows[i][csvData::Control]);
            tState.m_bStarter = rows[i][csvData::StarterRelief] == L"æ”­";
            tState.m_bEntry = false;
            tState.m_bLeave = false;
            tState.m_bBentch = true;
            tState.m_eAptitude = FieldingNo::Pitcher;
            tState.m_eFieldingNo = FieldingNo::None;
            if (tState.m_bStarter)
            {
                tState.m_nBenchNo = 0;
            }
            else
            {
                nPitcherBenchNo++;
                tState.m_nBenchNo = nPitcherBenchNo;
            }
            m_tVecPitcherMember.push_back(tState);
        }
        else if (rows[i][csvData::PitcherBatter] == L"–ìŽè")
        {
            BatterState tState;
            tState.m_sName = rows[i][csvData::Name];
            tState.m_nLineupNo = std::stoi(rows[i][csvData::LineupNo]);
            tState.m_nTrajectory = std::stoi(rows[i][csvData::Trajectory]);
            tState.m_eMeat = (Quality)std::stoi(rows[i][csvData::Meat]);
            tState.m_ePower = (Quality)std::stoi(rows[i][csvData::Power]);
            tState.m_eSpeed = (Quality)std::stoi(rows[i][csvData::Speed]);
            tState.m_eThrowing = (Quality)std::stoi(rows[i][csvData::Throwing]);
            tState.m_eDefence = (Quality)std::stoi(rows[i][csvData::Defence]);
            tState.m_eChatch = (Quality)std::stoi(rows[i][csvData::Chatch]);
            tState.m_eAptitude = (FieldingNo)std::stoi(rows[i][csvData::Position]);
            tState.m_eSubAptitude = (FieldingNo)std::stoi(rows[i][csvData::SubPosition]);
            tState.m_eFieldingNo = (FieldingNo)std::stoi(rows[i][csvData::PositionEntry]);
            tState.m_bLefty = rows[i][csvData::Handed] == L"¶";
            tState.m_bEntry = false;
            tState.m_bLeave = false;
            if (tState.m_eFieldingNo != 0)
            {
                tState.m_nBenchNo = 0;
            }
            else
            {
                nBatterBenchNo++;
                tState.m_nBenchNo = nBatterBenchNo;
            }
            m_tVecBatterMember.push_back(tState);
        }
        else
        {
            INFO_MESSAGE("NonePosition");
        }
    }
    return true;
}

CTeamManager::Teams CTeamManager::GetTeam()
{
    return m_eTeamKind;
}

void CTeamManager::InitStarter()
{
    int nRand = rand() % 6;
    int i = 0;
    int j = 0;

    for (auto itr = m_tVecPitcherMember.begin(); itr != m_tVecPitcherMember.end(); itr++)
    {
        if (!itr->m_bStarter)
        {
            itr->m_nStarterNo = -1;
            continue;
        }

        if (nRand == j)
        {
            itr->m_bEntry = true;
            itr->m_eFieldingNo = FieldingNo::Pitcher;
            itr->m_nStarterNo = -1;
        }
        else
        {
            itr->m_nStarterNo = i;
            i++;
        }
        j++;
    }
}

void CTeamManager::ResetStarter(int StarterNo)
{
    for (auto itr = m_tVecPitcherMember.begin(); itr != m_tVecPitcherMember.end(); itr++)
    {
        if (!itr->m_bStarter) continue;
        if (itr->m_bEntry)
        {
            for (auto itr2 = m_tVecPitcherMember.begin(); itr2 != m_tVecPitcherMember.end(); itr2++)
            {
                if (!itr2->m_bStarter) continue;
                if (itr2->m_nStarterNo == StarterNo)
                {
                    std::swap(itr->m_eFieldingNo, itr2->m_eFieldingNo);
                    std::swap(itr->m_nStarterNo, itr2->m_nStarterNo);
                    itr->m_bEntry = false;
                    itr2->m_bEntry = true;
                    break;
                }
            }
            break;
        }
    }


}

void CTeamManager::ResetLineup(int BenchNo, int inLineupNo)
{
    for (auto itr = m_tVecBatterMember.begin(); itr != m_tVecBatterMember.end(); itr++)
    {
        if (itr->m_nLineupNo == inLineupNo)
        {
            for (auto itr2 = m_tVecBatterMember.begin(); itr2 != m_tVecBatterMember.end(); itr2++)
            {
                if (itr2->m_nBenchNo == BenchNo)
                {
                    std::swap(itr->m_eFieldingNo, itr2->m_eFieldingNo);
                    std::swap(itr->m_nLineupNo, itr2->m_nLineupNo);
                    std::swap(itr->m_nBenchNo, itr2->m_nBenchNo);
                    itr->m_bEntry = false;
                    itr2->m_bEntry = true;
                    break;
                }
            }
            break;
        }
    }
}

void CTeamManager::ResetLineupNo(int LineupNo1, int LineupNo2)
{
    for (auto itr = m_tVecBatterMember.begin(); itr != m_tVecBatterMember.end(); itr++)
    {
        if (itr->m_nLineupNo == LineupNo1)
        {
            for (auto itr2 = m_tVecBatterMember.begin(); itr2 != m_tVecBatterMember.end(); itr2++)
            {
                if (itr2->m_nLineupNo == LineupNo2)
                {
                    std::swap(itr->m_nLineupNo, itr2->m_nLineupNo);
                    break;
                }
            }
            break;
        }
    }
}

void CTeamManager::ResetFielding(FieldingNo LineupNo1, FieldingNo LineupNo2)
{
    for (auto itr = m_tVecBatterMember.begin(); itr != m_tVecBatterMember.end(); itr++)
    {
        if (itr->m_eFieldingNo == LineupNo1)
        {
            for (auto itr2 = m_tVecBatterMember.begin(); itr2 != m_tVecBatterMember.end(); itr2++)
            {
                if (itr2->m_eFieldingNo == LineupNo2)
                {
                    std::swap(itr->m_eFieldingNo, itr2->m_eFieldingNo);
                    break;
                }
            }
            break;
        }
    }
}

void CTeamManager::SetEntry(int No, bool isEntry, bool isPitcher)
{
    if (isPitcher) m_tVecPitcherMember[No].m_bEntry = isEntry;
    else m_tVecBatterMember[No].m_bEntry = isEntry;
}

void CTeamManager::SetBenchNo(int No, int BenchNo, bool isPitcher)
{
    if (isPitcher) m_tVecPitcherMember[No].m_nBenchNo = BenchNo;
    else m_tVecBatterMember[No].m_nBenchNo = BenchNo;
}

void CTeamManager::SetPosition(int No, FieldingNo position, bool isPitcher)
{
    if (isPitcher) m_tVecPitcherMember[No].m_eFieldingNo = position;
    else m_tVecBatterMember[No].m_eFieldingNo = position;
}

void CTeamManager::SetPitcherState(std::vector<PitcherState> state)
{
    m_tVecPitcherMember = state;
}

void CTeamManager::SetBatterState(std::vector<BatterState> state)
{
    m_tVecBatterMember = state;
}

void CTeamManager::SetFourBall()
{
    int i = 0;
    for (auto itr = m_tVecBatterMember.begin(); itr != m_tVecBatterMember.end(); itr++, i++)
    {
        if (itr->m_nLineupNo == m_nTakingNo)
        {
            for (auto itr1 = m_tVecBatterMember.begin(); itr1 != m_tVecBatterMember.end(); itr1++)
            {
                if (itr1->m_nBaseNo == 1)
                {
                    for (auto itr2 = m_tVecBatterMember.begin(); itr2 != m_tVecBatterMember.end(); itr2++)
                    {
                        if (itr2->m_nBaseNo == 2)
                        {
                            for (auto itr3 = m_tVecBatterMember.begin(); itr3 != m_tVecBatterMember.end(); itr3++)
                            {
                                if (itr3->m_nBaseNo == 3)
                                {
                                    itr3->m_nBaseNo = 0;
                                    break;
                                }
                            }
                            itr2->m_nBaseNo = 3;
                            break;
                        }
                    }
                    itr1->m_nBaseNo = 2;
                    break;
                }
            }
            itr->m_nBaseNo = 1;
            break;
        }
    }
}

void CTeamManager::NextBatter()
{
    m_nTakingNo++;
    if (m_nTakingNo > 9) m_nTakingNo = 1;
}

std::shared_ptr<CTeamManager>& CTeamManager::GetInstance(int teamNo)
{
    if (!m_pTeam[teamNo])
    {
        m_pTeam[teamNo].reset(new CTeamManager());
        return m_pTeam[teamNo];
    }

    return m_pTeam[teamNo];
}

std::vector<CTeamManager::PitcherState> CTeamManager::GetPitcherState()
{
    return m_tVecPitcherMember;
}

std::vector<CTeamManager::BatterState> CTeamManager::GetBatterState()
{
    return m_tVecBatterMember;
}

CTeamManager::PitcherState CTeamManager::GetTakingPitcherState()
{
    int i = 0;
    for (auto itr = m_tVecPitcherMember.begin(); itr != m_tVecPitcherMember.end(); itr++,i++)
    {
        if (itr->m_bEntry)
        {
            return m_tVecPitcherMember[i];
        }
    }
}

CTeamManager::BatterState CTeamManager::GetTakingBatterState()
{
    int i = 0;
    for (auto itr = m_tVecBatterMember.begin(); itr != m_tVecBatterMember.end(); itr++, i++)
    {
        if (itr->m_nLineupNo == m_nTakingNo)
        {
            return m_tVecBatterMember[i];
        }
    }
}

CTeamManager::BatterState CTeamManager::GetRunnerState(int BaseNo)
{
    int i = 0;
    for (auto itr = m_tVecBatterMember.begin(); itr != m_tVecBatterMember.end(); itr++, i++)
    {
        if (itr->m_nBaseNo == BaseNo)
        {
            return m_tVecBatterMember[i];
        }
    }
}

CTeamManager::BatterState CTeamManager::GetFielderState(FieldingNo No)
{
    int i = 0;
    if (No != FieldingNo::Pitcher)
    {
        for (auto itr = m_tVecBatterMember.begin(); itr != m_tVecBatterMember.end(); itr++, i++)
        {
            if (itr->m_eFieldingNo == No)
            {
                return m_tVecBatterMember[i];
            }
        }
    }
    else
    {
        BatterState state;
        state.m_eDefence = Quality::D;
        state.m_eThrowing = Quality::D;
        state.m_eChatch = Quality::D;
        return state;
    }
}

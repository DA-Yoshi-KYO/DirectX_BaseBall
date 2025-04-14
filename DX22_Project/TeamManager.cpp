#include "TeamManager.h"

std::unique_ptr<CTeamManager> CTeamManager::m_pTeam[2] = { nullptr,nullptr };

void CTeamManager::Init()
{
    m_eTeamKind = Teams::Monkeys;

    for (auto itr = m_tVecBatterMember.begin(); itr != m_tVecBatterMember.end();)
    {
        m_tVecBatterMember.erase(itr);
        itr = m_tVecBatterMember.begin();
    }

    for (auto itr = m_tVecPitcherMember.begin(); itr != m_tVecPitcherMember.end();)
    {
        m_tVecPitcherMember.erase(itr);
        itr = m_tVecPitcherMember.begin();
    }
}

CTeamManager::CTeamManager()
    : m_eTeamKind(Teams::Monkeys)
    , m_tVecBatterMember{}, m_tVecPitcherMember{}
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
        return false;
    }

    fFile.imbue(std::locale("Japanese_Japan.932"));

    std::wstring line;
    std::vector<std::wstring> row;
    std::vector<std::vector<std::wstring>> rows;

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

    for (int i = 0; i < rows.size(); i++)
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
            tState.m_bEntry = false;
            tState.m_bLeave = false;
            tState.m_bBentch = true;
            tState.m_eAptitude = FieldingNo::Pitcher;
            tState.m_eFieldingNo = FieldingNo::None;
            m_tVecPitcherMember.push_back(tState);
        }
        else if (rows[i][csvData::PitcherBatter] == L"–ìŽè")
        {
            BatterState tState;
            tState.m_sName = rows[i][csvData::Name];
            tState.m_bLefty = rows[i][csvData::Handed] == L"¶";
            tState.m_nTrajectory = std::stoi(rows[i][csvData::Trajectory]);
            tState.m_eMeat = (Quality)std::stoi(rows[i][csvData::Meat]);
            tState.m_ePower = (Quality)std::stoi(rows[i][csvData::Power]);
            tState.m_eSpeed = (Quality)std::stoi(rows[i][csvData::Speed]);
            tState.m_eThrowing = (Quality)std::stoi(rows[i][csvData::Throwing]);
            tState.m_eDefence = (Quality)std::stoi(rows[i][csvData::Defence]);
            tState.m_eChatch = (Quality)std::stoi(rows[i][csvData::Chatch]);
            tState.m_eAptitude = (FieldingNo)std::stoi(rows[i][csvData::Position]);
            tState.m_eSubAptitude = (FieldingNo)std::stoi(rows[i][csvData::SubPosition]);
            tState.m_bEntry = false;
            tState.m_bLeave = false;
            tState.m_nLineupNo = 0;
            tState.m_eFieldingNo = FieldingNo::None;
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

std::unique_ptr<CTeamManager>& CTeamManager::GetInstance(int teamNo)
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

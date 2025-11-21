#include "Team.h"
#include <string>
#include <fstream>
#include <sstream>
#include "PitcherData.h"
#include "FielderData.h"
#include "ChatcherData.h"
#include "InFielderData.h"
#include "OutFielderData.h"
#include "Defines.h"
#include "DirectX.h"

enum csvData
{
    Name,			// 名前
    PitcherBatter,	// 投手か野手か
    Handed,			// 利き腕
    BallSpeed,		// 球速
    Control,		// コントロール
    Stamina,		// スタミナ
    Slider,			// スライダー変化量
    Curve,			// カーブ変化量
    Split,			// フォーク変化量
    Sinker,			// シンカー変化量
    Shoot,			// シュート変化量
    Twoseam,		// ツーシーム変化量
    SliderKind,		// スライダー系球種
    CurveKind,		// カーブ系球種
    SplitKind,		// フォーク系球種
    SinkerKind,		// シンカー変化量系球種
    ShootKind,		// シュート系球種
    TwoseamKind,	// ツーシーム系球種
    StarterRelief,	// 先発か中継ぎか
    Trajectory,		// 弾道
    Meat,			// ミート
    Power,			// パワー
    Speed,			// 走力
    Throwing,		// 肩力
    Defence,		// 守備力
    Chatch,			// 捕球
    Position,		// ポジション
    SubPosition,	// サブポジション
    PositionEntry,	// 出場ポジション
    LineupNo		// 打順
};

CTeam::CTeam()
{

}

CTeam::~CTeam()
{
    for (auto itr : m_pMemberData)
    {
        SAFE_DELETE(itr);
    }
}

void CTeam::Load(TeamKind team)
{
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
        ERROR_MESSAGE_WS(wsPath[(int)team].c_str());
        return;
    }

    std::locale jpLocale("Japanese_Japan.932");
    fFile.imbue(jpLocale);

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

    int nPitcherBenchNo = 0;
    int nBatterBenchNo = 0;

    for (int i = 1; i < rows.size(); i++)
    {
        PlayerData data;
        data.m_wsName = rows[i][csvData::Name];
        data.m_eHandy = rows[i][csvData::Handed] == L"左" ? Hand::Left : Hand::Right;
        data.m_eMainPosition = Positions(std::stoi(rows[i][csvData::Position]) - 1);
        data.m_nLineupNo = std::stoi(rows[i][csvData::LineupNo]);

        PitcherData pitcherData{};
        FielderData fielderData{};
        // 投手か野手か判定
        if (rows[i][csvData::PitcherBatter] == L"投手")
        {

            pitcherData.m_fSpeed = std::stof(rows[i][csvData::BallSpeed]);
            pitcherData.m_eStamina = (Quality)std::stoi(rows[i][csvData::Stamina]);
            pitcherData.m_eControl = (Quality)std::stoi(rows[i][csvData::Control]);
            pitcherData.m_eKind = rows[i][csvData::StarterRelief] == L"先発" ? PitcherKind::Starter : PitcherKind::Relief;
        }
        else if (rows[i][csvData::PitcherBatter] == L"野手")
        {

            fielderData.m_nTrajectory = std::stoi(rows[i][csvData::Trajectory]);
            fielderData.m_eMeet = (Quality)std::stoi(rows[i][csvData::Meat]);
            fielderData.m_ePower = (Quality)std::stoi(rows[i][csvData::Power]);
            fielderData.m_eSpeed = (Quality)std::stoi(rows[i][csvData::Speed]);
            fielderData.m_eThrowing = (Quality)std::stoi(rows[i][csvData::Throwing]);
            fielderData.m_eDefence = (Quality)std::stoi(rows[i][csvData::Defence]);
            fielderData.m_eChatch = (Quality)std::stoi(rows[i][csvData::Chatch]);
        }
        else
        {
            INFO_MESSAGE("NonePosition");
        }

        CPlayerDataBase* pDataBase = nullptr;
        CPitcherData* pPitcher = nullptr;
        CChatcherData* pChatcher = nullptr;
        CInFielderData* pInFielder = nullptr;
        COutFielderData* pOutFielder = nullptr;
        switch (data.m_eMainPosition)
        {
        case Positions::Pitcher:
            pDataBase = new CPitcherData();
            pDataBase->SetPlayerData(data);
            pPitcher = dynamic_cast<CPitcherData*>(pDataBase);
            pPitcher->SetPitcherData(pitcherData);
            m_pPitcherData.push_back(pPitcher);
            break;
        case Positions::Chatcher:
            pDataBase = new CChatcherData();
            pDataBase->SetPlayerData(data);
            pChatcher = dynamic_cast<CChatcherData*>(pDataBase);
            pChatcher->SetFielderData(fielderData);
            m_pFielderData.push_back(pChatcher);
            m_pChatcherData.push_back(pChatcher);
            break;
        case Positions::First:
        case Positions::Second:
        case Positions::Third:
        case Positions::Short:
            pDataBase = new CInFielderData();
            pDataBase->SetPlayerData(data);
            pInFielder = dynamic_cast<CInFielderData*>(pDataBase);
            pInFielder->SetFielderData(fielderData);
            m_pFielderData.push_back(pInFielder);
            m_pInFielderData.push_back(pInFielder);
            break;
        case Positions::Left:
        case Positions::Center:
        case Positions::Right:
            pDataBase = new COutFielderData();
            pDataBase->SetPlayerData(data);
            pOutFielder = dynamic_cast<COutFielderData*>(pDataBase);
            pOutFielder->SetFielderData(fielderData);
            m_pFielderData.push_back(pOutFielder);
            m_pOutFielderData.push_back(pOutFielder);
            break;
        }
        m_pMemberData.push_back(pDataBase);
    }
}

CFielderData* CTeam::GetTakingBatter(int TakingNo)
{
    int i = 1;
    for (auto itr : m_pStartingLineup)
    {
        if (i == TakingNo) return itr;
        ++i;
    }

    return nullptr;
}

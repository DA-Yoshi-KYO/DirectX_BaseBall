#include "TeamBears.h"

CTeamBears::CTeamBears()
{
}

CTeamBears::~CTeamBears()
{
}

bool CTeamBears::Load()
{
	std::wifstream fFile(PATH_DATA("Bears.csv"));

    if (!fFile)
    {
        ERROR_MESSAGE("Bears.csv");
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
        if (rows[i][1] == L"“ŠŽè")
        {
            PitcherState tState;
            tState.m_sName = rows[i][0];
            tState.m_sName = rows[i][0];
            tState.m_sName = rows[i][0];
        }
        else if (rows[i][1] == L"–ìŽè")
        {

        }
        else
        {
            INFO_MESSAGE("NonePosition");
        }
    }
    return true;
}

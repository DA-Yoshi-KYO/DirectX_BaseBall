#pragma once

#include <string>

enum class Quality
{
	G,
	F,
	E,
	D,
	C,
	B,
	A,
	S
};

enum class Hand
{
	Right,
	Left
};

enum class Positions
{
	Pitcher,
	Chatcher,
	First,
	Second,
	Third,
	Short,
	Left,
	Center,
	Right,

	Max,
	None
};

struct PlayerData
{
	std::wstring m_wsName;
	Hand m_eHandy;
	Positions m_eMainPosition;
	Positions m_eEntryPosition;
	int m_nLineupNo;
};

class CPlayerDataBase
{
public:
	CPlayerDataBase();
	virtual ~CPlayerDataBase();
	void SetPlayerData(PlayerData data) { m_tPlayerData = data; }
	PlayerData GetPlayerData() { return m_tPlayerData; }

protected:
	PlayerData m_tPlayerData;

};

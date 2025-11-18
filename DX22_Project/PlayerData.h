#pragma once

#include <string>

enum class Hand
{
	Right,
	Left
};

class CPlayerDataBase
{
protected:
	std::wstring m_nName;
	Hand m_eHandy;
};

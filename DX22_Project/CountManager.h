#pragma once

#include <array>

class CCountManager
{
public:
	CCountManager();
	~CCountManager();
	void Init();
	void Update();

	void AddStrikeCount();
	void AddBallCount();
	void AddOutCount();
	void SetIsBase(bool isBase, int index);

private:
	std::array<class CStrikeCount*, 2> m_pStrikeCount;
	std::array<class CBallCount*, 3> m_pBallCount;
	std::array<class COutCount*, 2> m_pOutCount;
	std::array<class CBaseCount*, 3> m_pBaseCount;
	std::array<class CScore*, 2> m_pScore;
	class CInning* m_pInning;

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


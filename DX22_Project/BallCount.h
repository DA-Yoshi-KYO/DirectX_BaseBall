#pragma once

#include "Defines.h"
#include "Texture.h"

#define MAX_BALL_COUNT 4
#define MAX_STRIKE_COUNT 3
#define MAX_OUT_COUNT 3
#define MAX_BASE_COUNT 4
#define MAX_SCORE 99
#define MAX_INNING 12

class CBallCount
{
private:
	CBallCount();
public:
	~CBallCount();
	void Init();
	void Update();
	void Draw();
	void AddBallCount();
	void AddStrikeCount();
	void AddOutCount();
	void SetBaseState(int base, bool state);
	void ResetCount();
	void ChangeInning();
	bool IsEnd();
	struct TCount
	{
		int m_nBallCount;
		int m_nStrikeCount;
		int m_nOutCount;
		bool m_bBaseState[4];
		int m_nScore[2];
		int m_nInning;
		bool m_bTop;
		bool m_bEnd;
	};
	TCount GetCount() const { return m_tCount; }

	enum class Inning
	{
		TOP,
		BOTTOM
	};

	enum class Team
	{
		TOP,
		BOTTOM
	};

	static std::unique_ptr <CBallCount>& GetInstance();
	static void DestroyInstance();

private:
	std::unique_ptr<Texture> m_pSheet;
	std::unique_ptr<Texture> m_pBack;
	SpriteParam m_tBallParam;
	SpriteParam m_tBackParam;
	TCount m_tCount;

	static std::unique_ptr<CBallCount> m_pInstance;
};
#pragma once

#include "GameObject.h"
#include "CollisionBox.h"

enum class RunnerKind
{
	BatterToFirst,
	StayFirst,
	FirstToSecond,
	SecondToFirst,
	StaySecond,
	SecondTo
};

enum class GotBase
{
	None = -1,
	First,
	Second,
	Third,

	Max
};

class CRunner : public CGameObject
{
public:
	CRunner();
	~CRunner();
	void Init() override;
	void SetRunnerParam();
	void GoToNextBase();
	GotBase GetNowBase() { return GotBase(m_nNowBase); }
	
private:
	int m_nNowBase;
	CCollisionBox* m_pCollision;

};


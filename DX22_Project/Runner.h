#pragma once

#include "GameObject.h"
#include "CollisionBox.h"
#include "Base.h"
#include <array>

enum class RunnerKind
{
	BatterToFirst,
	StayFirst,
	FirstToSecond,
	StaySecond,
	SecondToThird,
	StayThird,
	ThirdToHome
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
	void Update() override;
	void OnCollision(CCollisionBase* other, std::string thisTag, Collision::Result result) override;
	void SetRunnerParam();
	void CheckRunOut();
	void NotRunOut() { m_bRunOut = false; }
	void GoToNextBase();
	GotBase GetNowBase() { return m_eNowBase; }
	
private:
	GotBase m_eNowBase;
	GotBase m_eTempBase;
	bool m_bIsStop;
	bool m_bFrontMove;
	bool m_bRunOut;
	bool m_bBackTempBase;
	CCollisionBox* m_pCollision;
	std::array<DirectX::XMFLOAT3, int(BaseKind::Max)> m_f3TargetPos;
	RunnerKind m_eCurrentRunnerKind;

	void UpdateInput();
	void UpdateBackTempBase();
};


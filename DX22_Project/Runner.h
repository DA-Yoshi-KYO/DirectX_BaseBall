#pragma once

#include "GameObject.h"
#include "CollisionBox.h"
#include "Base.h"
#include <array>
#include "FielderData.h"

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
	void ResetPos();
	void SetRunnerSpeed(Quality speed);
	void SetFirstBaseRunner() { m_eNowBase = GotBase::First; m_eTempBase = GotBase::First; };
	void CheckRunOut();
	void NotRunOut() { m_bRunOut = false; }
	void GoToNextBase();
	void ToNormalRunner() { m_bBatterRunner = true; }
	GotBase GetNowBase() { return m_eNowBase; }
	RunnerKind GetCurrentKind() { return m_eCurrentRunnerKind; }
	bool GetIsBatterRunner() { return m_bBatterRunner; }

private:
	GotBase m_eNowBase;
	GotBase m_eTempBase;
	bool m_bIsStop;
	bool m_bFrontMove;
	bool m_bRunOut;
	bool m_bBackTempBase;
	bool m_bBatterRunner;
	float m_fStiffTime;
	float m_fSpeed;
	CCollisionBox* m_pCollision;
	std::array<DirectX::XMFLOAT3, int(BaseKind::Max)> m_f3TargetPos;
	RunnerKind m_eCurrentRunnerKind;

	void UpdateInput();
	void UpdateBackTempBase();
	bool CheckCanProgress();

};


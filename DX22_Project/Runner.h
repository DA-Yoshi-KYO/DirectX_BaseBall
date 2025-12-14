#pragma once

#include "GameObject.h"
#include "CollisionBox.h"
#include "Base.h"
#include <array>
#include "FielderData.h"

enum class RunnerStatus
{
	Stop,
	ToNext,
	ToBack,
	BackAll,

	Max
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
	void OnDestroy() override;
	void OnCollision(CCollisionBase* other, std::string thisTag, Collision::Result result) override;
	void OnCollisionExit(CCollisionBase* other, std::string thisTag) override;
	void ResetPos();
	void SetRunnerSpeed(Quality speed);
	void SetFirstBaseRunner() { m_eCurrentBase = GotBase::First; m_eOldBase = GotBase::First; }
	void NotRunOut() { m_bRunOut = false; }
	void GoToNextBase();
	void ToNormalRunner() { m_bBatterRunner = false; }
	GotBase GetNowBase() { return m_eCurrentBase; }
	GotBase GetTouchBase() { return m_eCurrentBase; }
	void SetStatus(RunnerStatus status) { m_eStatus = status; }
	
private:
	GotBase m_eCurrentBase;
	GotBase m_eOldBase;
	GotBase m_eTouchBase;
	RunnerStatus m_eStatus;
	bool m_bIsStop;
	bool m_bFrontMove;
	bool m_bRunOut;
	bool m_bBackTempBase;
	bool m_bBatterRunner;
	bool m_bAutoStart;
	bool m_bTightRunner;
	float m_fStiffTime;
	float m_fSpeed;
	CCollisionBox* m_pCollision;
	std::array<DirectX::XMFLOAT3, int(BaseKind::Max)> m_f3TargetPos;

	void UpdateInput();
	void UpdateBackTempBase();
	bool CheckCanProgress();
	void CheckRunnerAutoStart();
	void CheckRunOut();

	void UpdateToNext();
	void UpdateToBack();
};

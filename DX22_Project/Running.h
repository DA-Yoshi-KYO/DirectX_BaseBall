#pragma once

#include "GameObject.h"
#include "Runner.h"

class CRunning
{
public:
	CRunning();
	~CRunning();
	void Update(int AttackPlayer);
	void Draw();
	void SetBatterRunner(Quality speed);
	void AddFirstRunner(Quality speed);
	void RemoveRunner(CRunner* pThis);
	CRunner* GetBackRunner(CRunner* pRunner);

	bool IsAllBaseTight(BaseKind base);
	bool IsTight(CRunner* pThis);

	void ToNormalRunner();

	CRunner* GetBatterRunner() { return m_pBatterRunner; }
	CRunner* GetRunnerFromOldBase(GotBase base);
	std::vector<CRunner*> GetAllRunner() { return m_pAllRunners; }

private:
	CRunner* m_pBatterRunner;
	std::vector<CRunner*>  m_pRunners;
	std::vector<CRunner*>  m_pAllRunners;
	void TightRunnerNext(CRunner* pThis);

};

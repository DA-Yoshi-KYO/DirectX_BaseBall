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

	bool IsTight(CRunner* pThis);

	CRunner* GetBatterRunner() { return m_pBatterRunner; }
	std::vector<CRunner*> GetAllRunner() { return m_pRunners; }

private:
	CRunner* m_pBatterRunner;
	std::vector<CRunner*>  m_pRunners;
	std::vector<CRunner*>  m_pAllRunners;
	void TightRunnerNext(CRunner* pThis);

};

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

private:
	CRunner* m_pBatterRunner;
	std::vector<CRunner*>  m_pRunners;

};

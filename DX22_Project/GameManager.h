#pragma once

#include "CountManager.h"
#include "AttackManager.h"

class CGameManager
{
private:
	CGameManager();
	
public:
	~CGameManager();
	void Init();
	void Update();
	void Draw();

public:
	static CGameManager* GetInstance();
	CCountManager* GetCountManager() { return m_pCountManager; };
	CAttackManager* GetAttackManager() { return m_pAttackManager; };

private:
	static CGameManager* m_pInstance;
	CCountManager* m_pCountManager;
	CAttackManager* m_pAttackManager;

};
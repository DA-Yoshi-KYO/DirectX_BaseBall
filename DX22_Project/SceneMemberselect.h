#pragma once
#include "Scene.h"
#include "TeamManager.h"

class CSceneMemberselect : public CScene
{
public:
	CSceneMemberselect(CTeamManager::Teams player1 , CTeamManager::Teams player2);
	virtual ~CSceneMemberselect();
	virtual void Update() final;
	virtual void Draw() final;
	static std::unique_ptr<CTeamManager>& GetInstance(int teamNo);

private:
	static std::unique_ptr<CTeamManager> m_pTeam[2];
	void InitMember(CTeamManager::Teams team, int teamNo);
};

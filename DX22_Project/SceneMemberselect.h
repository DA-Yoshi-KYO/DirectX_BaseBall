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
private:
};

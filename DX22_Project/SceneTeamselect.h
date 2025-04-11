#pragma once

#include "Scene.h"
#include "TeamManager.h"

class CSceneTeamSelect : public CScene
{
public:
	CSceneTeamSelect();
	virtual ~CSceneTeamSelect();
	virtual void Update() final;
	virtual void Draw() final;

	static CTeamManager::Teams GetTeam(int TeamNo);
private:
	static int m_nTeam[2];
};
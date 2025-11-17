#pragma once

#include "Scene.h"
#include <memory>
#include "TeamSelectDirector.h"

class CSceneTeamSelect : public CScene
{
public:
	CSceneTeamSelect();
	~CSceneTeamSelect();
	void Init() final;
	void Update() final;

private:
	std::unique_ptr<CTeamSelectDirector> m_pTeamSelectDirector;
};
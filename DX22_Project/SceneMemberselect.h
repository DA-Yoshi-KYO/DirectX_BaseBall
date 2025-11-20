#pragma once

#include "Scene.h"
#include "Team.h"
#include <array>
#include "MemberSelectDirector.h"

class CSceneMemberselect : public CScene
{
public:
	CSceneMemberselect(TeamKind player1 , TeamKind player2);
	~CSceneMemberselect();
	void Init() final;
	void Update() final;

private:
	std::array<TeamKind,2> m_eTeamKind;
	std::unique_ptr<CMemberSelectDirector> m_pDirector;

};

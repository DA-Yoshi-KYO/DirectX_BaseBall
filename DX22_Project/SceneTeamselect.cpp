#include "SceneTeamselect.h"

CSceneTeamSelect::CSceneTeamSelect()
	: CScene()
{

}

CSceneTeamSelect::~CSceneTeamSelect()
{

}

void CSceneTeamSelect::Init()
{
	m_pTeamSelectDirector = std::unique_ptr<CTeamSelectDirector>();
}

void CSceneTeamSelect::Update()
{
	m_pTeamSelectDirector->Update();
	CScene::Update();
}

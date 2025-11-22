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
	m_pTeamSelectDirector = std::make_unique<CTeamSelectDirector>(); 
	m_pTeamSelectDirector->Init();
}

void CSceneTeamSelect::Update()
{
	m_pTeamSelectDirector->Update();
	CScene::Update();
}

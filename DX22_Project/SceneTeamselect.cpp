#include "SceneTeamselect.h"
#include "BGMSpeaker.h"

CSceneTeamSelect::CSceneTeamSelect()
	: CScene()
{

}

CSceneTeamSelect::~CSceneTeamSelect()
{

}

void CSceneTeamSelect::Init()
{
	AddGameObject<CBGMSpeaker>("BGM", Tag::Sound)->Init(SceneKind::TeamSelect);
	m_pTeamSelectDirector = std::make_unique<CTeamSelectDirector>(); 
	m_pTeamSelectDirector->Init();
}

void CSceneTeamSelect::Update()
{
	m_pTeamSelectDirector->Update();
	CScene::Update();
}

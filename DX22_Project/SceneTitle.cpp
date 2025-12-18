#include "SceneTitle.h"
#include "Input.h"
#include "Main.h"
#include "Sprite.h"
#include "Camera.h"
#include "BGMSpeaker.h"

CSceneTitle::CSceneTitle()
	: CScene()
{

}

CSceneTitle::~CSceneTitle()
{

}

void CSceneTitle::Init()
{
	AddGameObject<CBGMSpeaker>("BGM", Tag::Sound)->Init(SceneKind::Title);
	m_pDirector = std::make_unique<CTitleDirector>();
	m_pDirector->Init();
}

void CSceneTitle::Update()
{
	m_pDirector->Update();

	CScene::Update();
}

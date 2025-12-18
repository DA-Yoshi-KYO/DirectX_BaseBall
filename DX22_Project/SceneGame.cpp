// includes
#include "SceneGame.h"
#include "GameManager.h"
#include "Camera.h"
#include "BGMSpeaker.h"

CSceneGame::CSceneGame()
{

}

CSceneGame::~CSceneGame()
{

}

void CSceneGame::Init()
{
    AddGameObject<CBGMSpeaker>("BGM", Tag::Sound)->Init(SceneKind::Game);
    CGameManager::GetInstance()->Init();
    CCamera::GetInstance()->SetCameraKind(CameraKind::CAM_BATTER);
}

void CSceneGame::Update()
{
    CGameManager::GetInstance()->Update();
    CScene::Update();
}


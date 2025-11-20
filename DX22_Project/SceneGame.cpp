// includes
#include "SceneGame.h"
#include "GameManager.h"
#include "Camera.h"

CSceneGame::CSceneGame()
{

}

CSceneGame::~CSceneGame()
{

}

void CSceneGame::Init()
{
	CGameManager::GetInstance()->Init();
	CCamera::GetInstance()->SetCameraKind(CameraKind::CAM_BATTER);
}

void CSceneGame::Update()
{
	CGameManager::GetInstance()->Update();
	CSceneGame::Update();
}


// includes
#include "SceneGame.h"
#include "Main.h"
#include "Sprite.h"
#include "EffectGoal.h"
#include "CameraDebug.h"
#include "CameraEvent.h"
#include "CameraMinimap.h"
#include "CameraBatter.h"
#include "BallCount.h"
#include "StrikeZone.h"
#include "GameManager.h"

/*───────四大処理───────*/
// コンストラクタ
CSceneGame::CSceneGame()
{

}

// デストラクタ
CSceneGame::~CSceneGame()
{

}

void CSceneGame::Init()
{
	CGameManager::GetInstance()->Init();
	CCamera::GetInstance()->SetCameraKind(CameraKind::CAM_BATTER);
}

// 更新
void CSceneGame::Update()
{
	CGameManager::GetInstance()->Update();
	CSceneGame::Update();
}


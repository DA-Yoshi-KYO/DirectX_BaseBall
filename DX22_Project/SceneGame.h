#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#define MAX_TEST (8)

enum CameraKind 
{
	CAM_DEBUG,
	CAM_PLAYER,
	CAM_EVENT, // イベント用の定義 
	CAM_MINIMAP, // ミニマップ用の定義
	MAX_CAMERA // カメラ最大数 
};

// includes
#include "Camera.h"
#include "Player.h"
#include "Object.h"
#include "Field.h"
#include "GaugeUI.h"
#include "Scene.h"
#include "Goal.h"
#include "Effect.h"
#include "Minimap.h"
#include "Ball.h"
#include "BallUI.h"

// class
class CSceneGame : public CScene
{
	/*---------------四大処理---------------*/
public:
	CSceneGame();		// 初期化処理
	~CSceneGame();		// 終了処理
	void Update() final;		// 更新処理
	void Draw() final;		// 描画処理

	/*------------コンポジション------------*/
private:
	CCamera* m_pCamera[MAX_CAMERA]; // カメラ情報

	CPlayer* m_pPlayer;	// プレイヤー
	CObject* m_pObject;	// オブジェクト
	CField* m_pField;	// フィールド
	CGaugeUI* m_pGaugeUI;	// ゲージ
	CGoal* m_pGoal;		// ゴール
	Effect* m_pEffect;	// エフェクト
	CMinimap* m_pMinimap;	// ミニマップ
	CBall* m_pBall;		// ボール
	CBallUI* m_pBallUI;

	int m_nGoalCount;
	bool m_bEnd;
	CameraKind m_eCameraKind;

	/*---------------内部処理---------------*/
	void CameraUpdate();
	void DrawMinimap();
};

#endif // __SCENE_GAME_H__
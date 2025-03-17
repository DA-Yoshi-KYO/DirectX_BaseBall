#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#define MAX_TEST (8)

// includes
#include "Camera.h"
#include "Field.h"
#include "Scene.h"
#include "Effect.h"
#include "Minimap.h"
#include "Attack.h"
#include "Defence.h"

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
	std::unique_ptr<CField> m_pField;
	std::unique_ptr<CAttack> m_pAttack;
	std::unique_ptr<CDefence> m_pDefence;
	static CameraKind m_eCameraKind;
	static int m_nPlaying;

	/*---------------内部処理---------------*/
	void CameraUpdate();
	void DrawMinimap();

public:
	enum class Playing
	{
		Attack,
		Defence
	};
	static void SetPlaying(Playing playing);
	static Playing GetPlaying();
	static CameraKind GetCameraKind();
};

#endif // __SCENE_GAME_H__
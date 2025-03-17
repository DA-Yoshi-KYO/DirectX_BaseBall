// includes
#include "SceneGame.h"
#include "Main.h"
#include "Sprite.h"
#include "EffectGoal.h"
#include "CameraDebug.h"
#include "CameraEvent.h"
#include "CameraMinimap.h"
#include "CameraPlayer.h"
#include "CameraBatter.h"
#include "BallCount.h"

int CSceneGame::m_nPlaying = 0;
CameraKind CSceneGame::m_eCameraKind = CAM_BATTER;

/*───────四大処理───────*/
// コンストラクタ
CSceneGame::CSceneGame()
	: m_pAttack(nullptr), m_pDefence(nullptr)
{
	// 静的変数の初期化
	m_nPlaying = (int)Playing::Attack;
	CBallCount::GetInstance()->Init();

	// 各種初期化処理
	m_pField = std::make_unique<CField>();
	m_pAttack = std::make_unique<CAttack>();
	m_pDefence = std::make_unique<CDefence>();

	// カメラ


	// エフェクト
	//CEffectGoal* pEffect = new CEffectGoal();
	//pEffect->SetCamera(m_pCamera[m_eCameraKind]);	
	//m_pEffect = pEffect;							// エフェクトの設定

	// ミニマップ
	//m_pMinimap = new CMinimap();
	//
	//m_pPlayer->SetCamera(m_pCamera[m_eCameraKind]);
	//m_pField->SetCamera(m_pCamera[m_eCameraKind]);

	 // インスタンスのセット
	m_pField->SetCamera(CCamera::GetInstance(m_eCameraKind).get());
}

// デストラクタ
CSceneGame::~CSceneGame()
{

}

// 更新
void CSceneGame::Update()
{
	// カメラの切り替え処理
	CameraUpdate();

	m_pField->Update();		// フィールド
	switch (m_nPlaying)
	{
	case (int)Playing::Attack:
		m_pAttack->Update();
		m_pDefence->Update();
		break;
	case (int)Playing::Defence:
		m_pDefence->Update();
		break;
	default:
		break;
	}
	CBallCount::GetInstance()->Update();

}

// 描画
void CSceneGame::Draw()
{
	CCamera* pCamera = CCamera::GetInstance(m_eCameraKind).get();
	//DrawMinimap();

	// GeometoryへのView,Projection設定
	Geometory::SetView(pCamera->GetViewMatrix());
	Geometory::SetProjection(pCamera->GetProjectionMatrix());

	// SpriteへのView,Projection設定
	Sprite::SetView(pCamera->GetViewMatrix());
	Sprite::SetProjection(pCamera->GetProjectionMatrix());
	// 3D描画
	// レンダーターゲットを3D用の設定に変更
	SetRender3D();

	m_pField->Draw();	// フィールドの描画
	switch (m_nPlaying)
	{
	case (int)Playing::Attack:
		m_pAttack->Draw();
		m_pDefence->Draw();
		break;
	case (int)Playing::Defence:
		m_pDefence->Draw();
		break;
	default:
		break;
	}
	CBallCount::GetInstance()->Draw();

	//m_pPlayer->Draw();	// プレイヤーの描画

	// 2D描画
	// レンダーターゲットを2D用の設定に変更
	SetRender2D();
	//m_pMinimap->Draw();	// ミニマップの描画
	//if (m_pEffect)m_pEffect->Draw();	// エフェクトの描画
}

CameraKind CSceneGame::GetCameraKind()
{
	return m_eCameraKind;
}

/*───────内部処理───────*/
void CSceneGame::CameraUpdate()
{
	// カメラの更新
	CCamera::GetInstance(m_eCameraKind)->Update();

	// イベント用のカメラ情報を取得
}

void CSceneGame::DrawMinimap()
{
	CCamera* pCamera = CCamera::GetInstance(m_eCameraKind).get();

	// ミニマップ用カメラの更新
	pCamera->Update(); // 更新処理だが描画で実行  
	// 各種カメラをミニマップ用に設定
	m_pField->SetCamera(pCamera);
	//((CEffectGoal*)m_pEffect)->SetCamera(m_pCamera[CAM_MINIMAP]);

	// ミニマップ表示用の変換行列を取得
	DirectX::XMFLOAT4X4 view = pCamera->GetViewMatrix();
	DirectX::XMFLOAT4X4 proj = pCamera->GetProjectionMatrix();
	// Geometoryに設定
	Geometory::SetView(view);
	Geometory::SetProjection(proj);
	// Spriteに設定
	Sprite::SetView(view);
	Sprite::SetProjection(proj);

	// ミニマップの作成開始
	//m_pMinimap->BeginRender();

	// ミニマップに表示するものを描画
	m_pField->Draw();
	//m_pEffect->Draw();

	// ミニマップの作成終了
	//m_pMinimap->EndRender();

	// 各種カメラを元に戻す
	m_pField->SetCamera(pCamera);
	//((CEffectGoal*)m_pEffect)->SetCamera(m_pCamera[m_eCameraKind]);
}

void CSceneGame::SetPlaying(Playing playing)
{
	m_nPlaying = (int)playing;
}

CSceneGame::Playing CSceneGame::GetPlaying()
{
	return (Playing)m_nPlaying;
}

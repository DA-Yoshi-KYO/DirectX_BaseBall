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

/*───────四大処理───────*/
// コンストラクタ
CSceneGame::CSceneGame()
	: m_pAttack(nullptr), m_pDefence(nullptr)
{

	// 各種初期化処理
	CBallCount::GetInstance()->Init(CBallCount::InningHalf::Top);
	m_pAttack = std::make_unique<CAttack>();
	m_pDefence = std::make_unique<CDefence>();

	// カメラ
#ifdef _CAM_DEBUG
	CCamera::SetCameraKind(CameraKind::CAM_DEBUG);
#else
	if (1)
	{
		CCamera::SetCameraKind(CameraKind::CAM_BATTER);
	}
	else
	{

	}
#endif // DEBUG


	// エフェクト
	//CEffectGoal* pEffect = new CEffectGoal();
	//pEffect->SetCamera(m_pCamera[m_eCameraKind]);	
	//m_pEffect = pEffect;							// エフェクトの設定

	// ミニマップ
	//m_pMinimap = new CMinimap();
	//
	//m_pPlayer->SetCamera(m_pCamera[m_eCameraKind]);
	//m_pField->SetCamera(m_pCamera[m_eCameraKind]);

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

	CField::GetInstance()->Update();		// フィールド
	CStrikeZone::GetInstance()->Update();
	m_pDefence->Update();
	m_pAttack->Update();

	CBall::GetInstance()->Update();
	CBallCount::GetInstance()->Update();

}

// 描画
void CSceneGame::Draw()
{
	CCamera* pCamera = CCamera::GetInstance(CCamera::GetCameraKind()).get();
	CBall* pBall = CBall::GetInstance().get();
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

	CField::GetInstance()->Draw();	// フィールドの描画
	if (pBall->GetPhase() == CBall::BallPhase::Batting)
	{
		CStrikeZone::GetInstance()->Draw();
	}
	m_pDefence->Draw();
	m_pAttack->Draw();

	pBall->Draw();
	if (pBall->GetPhase() == CBall::BallPhase::Batting)
	{
		CBallCount::GetInstance()->Draw();
	}
	//m_pPlayer->Draw();	// プレイヤーの描画

	// 2D描画
	// レンダーターゲットを2D用の設定に変更
	SetRender2D();
	//m_pMinimap->Draw();	// ミニマップの描画
	//if (m_pEffect)m_pEffect->Draw();	// エフェクトの描画
}

/*───────内部処理───────*/
void CSceneGame::CameraUpdate()
{
	// カメラの更新
	CCamera::GetInstance(CCamera::GetCameraKind())->Update();

	// イベント用のカメラ情報を取得
}

void CSceneGame::DrawMinimap()
{
	CCamera* pCamera = CCamera::GetInstance(CCamera::GetCameraKind()).get();

	// ミニマップ用カメラの更新
	pCamera->Update(); // 更新処理だが描画で実行  
	// 各種カメラをミニマップ用に設定
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
	CField::GetInstance()->Draw();
	//m_pEffect->Draw();

	// ミニマップの作成終了
	//m_pMinimap->EndRender();

	// 各種カメラを元に戻す
	//((CEffectGoal*)m_pEffect)->SetCamera(m_pCamera[m_eCameraKind]);
}

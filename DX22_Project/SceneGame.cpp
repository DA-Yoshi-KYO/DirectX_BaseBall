// includes
#include "SceneGame.h"
#include "Main.h"
#include "Sprite.h"
#include "EffectGoal.h"
#include "CameraDebug.h"
#include "CameraEvent.h"
#include "CameraMinimap.h"
#include "CameraPlayer.h"

/*───────四大処理───────*/
// コンストラクタ
CSceneGame::CSceneGame()
	// メンバ変数の初期化
	: m_pCamera{}
	, m_pPlayer(nullptr), m_pField(nullptr), m_pObject(nullptr)
	, m_pGoal(nullptr), m_pEffect(nullptr), m_pGaugeUI(nullptr)
	, m_pMinimap(nullptr), m_pBall(nullptr), m_pBallUI(nullptr)
	, m_eCameraKind(CAM_EVENT), m_nGoalCount(0), m_bEnd(false)
{
	// 各種初期化処理
	// オブジェクト
	m_pPlayer = new CPlayer();
	//m_pObject = new CObject();
	m_pField = new CField();
	m_pGaugeUI = new CGaugeUI();
	m_pGoal = new CGoal();
	m_pBall = new  CBall();
	m_pBallUI = new CBallUI();

	// カメラ
	CCameraDebug* pCamDebug = new CCameraDebug();
	CCameraPlayer* pCamPlayer = new CCameraPlayer();
	pCamPlayer->SetPlayer(m_pPlayer);
	CCameraEvent* pCamEvent = new CCameraEvent();
	pCamEvent->SetEvent(DirectX::XMFLOAT3(0.0f, 5.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 25.0f, 43.3f), 5.0f);	// イベントの設定をする
	CCameraMinimap* pCamMinimap = new CCameraMinimap();
	pCamMinimap->SetPlayer(m_pPlayer);	// カメラで追いかけるプレイヤーを設定
	m_pCamera[CAM_DEBUG] = pCamDebug;
	m_pCamera[CAM_PLAYER] = pCamPlayer;
	m_pCamera[CAM_EVENT] = pCamEvent;
	m_pCamera[CAM_MINIMAP] = pCamMinimap;

	// エフェクト
	CEffectGoal* pEffect = new CEffectGoal();
	pEffect->SetCamera(m_pCamera[m_eCameraKind]);	
	m_pEffect = pEffect;							// エフェクトの設定

	// ミニマップ
	m_pMinimap = new CMinimap();
	
	m_pPlayer->SetCamera(m_pCamera[m_eCameraKind]);
	m_pPlayer->SetBall(m_pBall);
//	m_pObject->SetCamera(m_pCamera[m_eCameraKind]);
	m_pField->SetCamera(m_pCamera[m_eCameraKind]);	
	m_pGoal->SetCamera(m_pCamera[m_eCameraKind]);
	m_pBall->SetCamera(m_pCamera[m_eCameraKind]);
	m_pBall->SetPlayer(m_pPlayer);
	m_pBall->SetRandX();
}

// デストラクタ
CSceneGame::~CSceneGame()
{
	// 各種リリース処理
	// カメラ情報の解放
	for (int i = 0; i < MAX_CAMERA; i++)
	{
		SAFE_DELETE(m_pCamera[i]);
	}
	// プレイヤー情報の解放
	SAFE_DELETE(m_pPlayer);
	// オブジェクト情報の解放
	//SAFE_DELETE(m_pObject);
	// フィールド情報の解放
	SAFE_DELETE(m_pField);
	// ゲージ情報の解放
	SAFE_DELETE(m_pGaugeUI);
	// ゴール情報の解放
	SAFE_DELETE(m_pGoal);
	// エフェクト情報の解放
	SAFE_DELETE(m_pEffect);
	// ミニマップ情報の解放
	SAFE_DELETE(m_pMinimap);
}

// 更新
void CSceneGame::Update()
{
	// カメラの切り替え処理
	CameraUpdate();

	// エフェクトの再生中は更新処理を行わない
	if (!m_pEffect->IsPlay() && m_eCameraKind == CAM_PLAYER && !m_bEnd)
	{
		// 各種更新処理
		m_pField->Update();		// フィールド
		//m_pObject->Update();	// オブジェクト
		m_pPlayer->Update();	// プレイヤー
		m_pGoal->Update();		// ゴール
		m_pBall->Update();
		m_pBallUI->Update();
		// ゲージをパワーに応じて設定
		m_pGaugeUI->SetGauge(m_pPlayer->GetPower());

		//// 当たり判定情報
		//Collision::Info collisionPlayer;	// プレイヤー
		//Collision::Info collisionObject;	// オブジェクト

		//collisionPlayer = m_pPlayer->GetCollision();
		//// マップのサイズ分当たり判定の確認を行う
		//for (int j = 0; j < MAP_Y; j++)
		//{
		//	for (int i = 0; i < MAP_X; i++)
		//	{
		//		//　当たり判定情報の取得
		//		collisionObject = m_pObject->GetCollision(j, i);
		//		// プレイヤーとオブジェクトの当たり判定を確認
		//		Collision::Result result_PlayerToObject = Collision::Hit(collisionPlayer, collisionObject);
		//		// プレイヤーとオブジェクトが当たっている場合
		//		if (result_PlayerToObject.isHit)
		//		{
		//			result_PlayerToObject.other = collisionObject;	// 当たった先をオブジェクトとする
		//			m_pPlayer->OnCollision(result_PlayerToObject);	// プレイヤーの当たり判定処理を行う
		//		}

		//		// 影の位置を計算 
		//		if (collisionPlayer.type == Collision::eBox)
		//		{
		//			// 高さを考慮せず、Ｂｏｘの中心までの距離を判定 
		//			DirectX::XMFLOAT2 dist(
		//				fabsf(collisionPlayer.box.center.x - collisionObject.box.center.x),
		//				fabsf(collisionPlayer.box.center.z - collisionObject.box.center.z)
		//			);
		//			// Ｂｏｘ内に収まっているか判定 
		//			if (dist.x <= collisionObject.box.size.x * 0.5f &&
		//				dist.y <= collisionObject.box.size.z * 0.5f)
		//			{
		//				// Ｂｏｘの天面の座標を、影の位置に設定 
		//				m_pPlayer->SetShadow(DirectX::XMFLOAT3(
		//					collisionPlayer.box.center.x,
		//					collisionObject.box.center.y + collisionObject.box.size.y * 0.5f,
		//					collisionPlayer.box.center.z));
		//			}
		//		}
		//	}

		Collision::Info collisionPlayer = m_pPlayer->GetCollision();
		Collision::Info collisionBall = m_pBall->GetCollision();
		// プレイヤーとオブジェクトの当たり判定を確認
		Collision::Result result_PlayerToObject = Collision::Hit(collisionBall, collisionPlayer);
		// プレイヤーとオブジェクトが当たっている場合
		if (result_PlayerToObject.isHit && (m_pBall->GetPhase() != 1 || m_pBall->GetPhase() != 2))
		{
			m_pBall->OnCollision(result_PlayerToObject);	// プレイヤーの当たり判定処理を行う
		}
		// ゴールに当たった場合
		if (m_pGoal->IsHit(m_pBall->GetCollision()))
		{
			if (!m_pEffect->IsPlay())
			{
				// プレイヤーの位置にエフェクトを設定
				m_pEffect->SetPos({ -120.0f + WORLD_AJUST,0.0f + WORLD_AJUST,80.0f + WORLD_AJUST });
				// エフェクトの再生
				m_pEffect->Play();
				m_nGoalCount++;
				m_pBall->SetPos({ 0.0f + WORLD_AJUST,3.0f + WORLD_AJUST,180.0f + WORLD_AJUST });
				m_pBall->SetPhase(0);
				m_pPlayer->SetPos({ 0.0f + WORLD_AJUST,10.0f + WORLD_AJUST,20.0f + WORLD_AJUST });
				m_pBall->SetRandX();
			}
		}
		else if (m_pBall->GetPos().z <= -100.0f + WORLD_AJUST)
		{
			m_nGoalCount++;
			m_pBall->SetPos({ 0.0f + WORLD_AJUST,3.0f + WORLD_AJUST,180.0f + WORLD_AJUST });
			m_pBall->SetPhase(0);
			m_pPlayer->SetPos({ 0.0f + WORLD_AJUST,10.0f + WORLD_AJUST,20.0f + WORLD_AJUST });
			m_pBall->SetRandX();
		}
		if (m_nGoalCount >= 5)
		{
			m_bEnd = true;
			SetNext(0);
		}
		m_pBallUI->SetBallVal(MAX_BALL - 1 - m_nGoalCount);
	}
	


	if (m_pEffect)m_pEffect->Update();	// エフェクトの更新
}

// 描画
void CSceneGame::Draw()
{
	DrawMinimap();

	// GeometoryへのView,Projection設定
	Geometory::SetView(m_pCamera[m_eCameraKind]->GetViewMatrix());
	Geometory::SetProjection(m_pCamera[m_eCameraKind]->GetProjectionMatrix());

	// SpriteへのView,Projection設定
	Sprite::SetView(m_pCamera[m_eCameraKind]->GetViewMatrix());
	Sprite::SetProjection(m_pCamera[m_eCameraKind]->GetProjectionMatrix());

	// 3D描画
	// レンダーターゲットを3D用の設定に変更
	SetRender3D();
	m_pField->Draw();	// フィールドの描画
//	m_pObject->Draw();	// オブジェクトの描画	
	m_pPlayer->Draw();	// プレイヤーの描画
	m_pGoal->Draw();	// ゴールの描画
	m_pBall->Draw();

	// 2D描画
	// レンダーターゲットを2D用の設定に変更
	SetRender2D();
	m_pMinimap->Draw();	// ミニマップの描画
	m_pGaugeUI->Draw();	// ゲージの描画
	m_pBallUI->Draw();
	if (m_pEffect)m_pEffect->Draw();	// エフェクトの描画
}

/*───────内部処理───────*/
void CSceneGame::CameraUpdate()
{
	// カメラの更新
	m_pCamera[m_eCameraKind]->Update();

	// イベント用のカメラ情報を取得
	CCameraEvent* pEvent;

	switch (m_eCameraKind)
	{
	case CAM_PLAYER:
		// イベントをもう一度再生
		if (IsKeyTrigger('3'))
		{
			// イベントの情報を取得
			pEvent = reinterpret_cast<CCameraEvent*>(m_pCamera[CAM_EVENT]);
			// イベントの再生開始
			pEvent->SetEvent(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), m_pCamera[CAM_PLAYER]->GetPos(), 1.0f);
			// イベントカメラに切り替え
			m_eCameraKind = CAM_EVENT;
			// 各種カメラ情報の再設定
			m_pPlayer->SetCamera(m_pCamera[m_eCameraKind]);
			//m_pObject->SetCamera(m_pCamera[m_eCameraKind]);
			m_pField->SetCamera(m_pCamera[m_eCameraKind]);
			m_pGoal->SetCamera(m_pCamera[m_eCameraKind]);
			((CEffectGoal*)m_pEffect)->SetCamera(m_pCamera[m_eCameraKind]);
		}
		break;
	case CAM_EVENT:
		// イベントの情報を取得
		pEvent = reinterpret_cast<CCameraEvent*>(m_pCamera[m_eCameraKind]);
		// イベントが終了したら
		if (!pEvent->IsEvent())
		{
			// プレイヤーカメラに切り替え
			m_eCameraKind = CAM_PLAYER;
			// 各種カメラ情報の再設定
			m_pPlayer->SetCamera(m_pCamera[m_eCameraKind]);
			//m_pObject->SetCamera(m_pCamera[m_eCameraKind]);
			m_pField->SetCamera(m_pCamera[m_eCameraKind]);
			m_pGoal->SetCamera(m_pCamera[m_eCameraKind]);
			((CEffectGoal*)m_pEffect)->SetCamera(m_pCamera[m_eCameraKind]);
		}
		break;
	case CAM_MINIMAP:
		break;
	case MAX_CAMERA:
		break;
	default:
		break;
	}

}

void CSceneGame::DrawMinimap()
{
	// ミニマップ用カメラの更新
	m_pCamera[CAM_MINIMAP]->Update(); // 更新処理だが描画で実行  
	// 各種カメラをミニマップ用に設定
	m_pPlayer->SetCamera(m_pCamera[CAM_MINIMAP]);
	//m_pObject->SetCamera(m_pCamera[CAM_MINIMAP]);
	m_pField->SetCamera(m_pCamera[CAM_MINIMAP]);
	m_pGoal->SetCamera(m_pCamera[CAM_MINIMAP]);
	m_pBall->SetCamera(m_pCamera[CAM_MINIMAP]);
	((CEffectGoal*)m_pEffect)->SetCamera(m_pCamera[CAM_MINIMAP]);

	// ミニマップ表示用の変換行列を取得
	DirectX::XMFLOAT4X4 view = m_pCamera[CAM_MINIMAP]->GetViewMatrix();
	DirectX::XMFLOAT4X4 proj = m_pCamera[CAM_MINIMAP]->GetProjectionMatrix();
	// Geometoryに設定
	Geometory::SetView(view);
	Geometory::SetProjection(proj);
	// Spriteに設定
	Sprite::SetView(view);
	Sprite::SetProjection(proj);

	// ミニマップの作成開始
	m_pMinimap->BeginRender();

	// ミニマップに表示するものを描画
	m_pPlayer->Draw();
	m_pField->Draw();
	//m_pObject->Draw();
	m_pGoal->Draw();
	m_pEffect->Draw();
	m_pBall->Draw();

	// ミニマップの作成終了
	m_pMinimap->EndRender();

	// 各種カメラを元に戻す
	m_pPlayer->SetCamera(m_pCamera[m_eCameraKind]);
	//m_pObject->SetCamera(m_pCamera[m_eCameraKind]);
	m_pField->SetCamera(m_pCamera[m_eCameraKind]);
	m_pGoal->SetCamera(m_pCamera[m_eCameraKind]);
	m_pBall->SetCamera(m_pCamera[m_eCameraKind]);
	((CEffectGoal*)m_pEffect)->SetCamera(m_pCamera[m_eCameraKind]);
}

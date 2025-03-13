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

int CSceneGame::m_nPlaying = 0;

/*───────四大処理───────*/
// コンストラクタ
CSceneGame::CSceneGame()
	// メンバ変数の初期化
	: m_pCamera{}
	, m_pPlayer(nullptr), m_pField(nullptr), m_pObject(nullptr)
	, m_pGoal(nullptr), m_pEffect(nullptr), m_pGaugeUI(nullptr)
	, m_pMinimap(nullptr), m_pBall(nullptr), m_pBallUI(nullptr)
	, m_pAttack(nullptr), m_pDefence(nullptr)
	, m_eCameraKind(CAM_BATTER), m_nGoalCount(0), m_bEnd(false)
{
	// 各種初期化処理
	// オブジェクト
	m_pPlayer = new CPlayer();
	m_pField = new CField();

	// カメラ
	CCameraDebug* pCamDebug = new CCameraDebug();
	CCameraPlayer* pCamPlayer = new CCameraPlayer();
	pCamPlayer->SetPlayer(m_pPlayer);
	CCameraEvent* pCamEvent = new CCameraEvent();
	pCamEvent->SetEvent(DirectX::XMFLOAT3(0.0f, 5.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 25.0f, 43.3f), 5.0f);	// イベントの設定をする
	CCameraMinimap* pCamMinimap = new CCameraMinimap();
	pCamMinimap->SetPlayer(m_pPlayer);	// カメラで追いかけるプレイヤーを設定
	CCameraBatter* pCamBatter = new CCameraBatter();

	m_pCamera[CAM_DEBUG] = pCamDebug;
	m_pCamera[CAM_PLAYER] = pCamPlayer;
	m_pCamera[CAM_EVENT] = pCamEvent;
	m_pCamera[CAM_MINIMAP] = pCamMinimap;
	m_pCamera[CAM_BATTER] = pCamBatter;

	// エフェクト
	CEffectGoal* pEffect = new CEffectGoal();
	pEffect->SetCamera(m_pCamera[m_eCameraKind]);	
	m_pEffect = pEffect;							// エフェクトの設定

	// ミニマップ
	m_pMinimap = new CMinimap();
	
	m_pPlayer->SetCamera(m_pCamera[m_eCameraKind]);
	m_pField->SetCamera(m_pCamera[m_eCameraKind]);

	m_nPlaying = (int)Playing::Attack;
	m_pAttack = std::make_unique<CAttack>();
	m_pDefence = std::make_unique<CDefence>();
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

	
	m_pField->Update();		// フィールド
	switch (m_nPlaying)
	{
	case (int)Playing::Attack:
		m_pAttack->Update();
		break;
	case (int)Playing::Defence:
		m_pDefence->Update();
		break;
	default:
		break;
	}

}

// 描画
void CSceneGame::Draw()
{
	//DrawMinimap();

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

	switch (m_nPlaying)
	{
	case (int)Playing::Attack:
		m_pAttack->Draw();
		break;
	case (int)Playing::Defence:
		m_pDefence->Draw();
		break;
	default:
		break;
	}

	//m_pPlayer->Draw();	// プレイヤーの描画

	// 2D描画
	// レンダーターゲットを2D用の設定に変更
	SetRender2D();
	m_pMinimap->Draw();	// ミニマップの描画
	//if (m_pEffect)m_pEffect->Draw();	// エフェクトの描画
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

void CSceneGame::SetPlaying(Playing playing)
{
	m_nPlaying = (int)playing;
}

Playing CSceneGame::GetPlaying()
{
	return (Playing)m_nPlaying;
}


/*+===================================================================
	File: Pitching.cpp
	Summary: 投球処理
	Author: 吉田京志郎
	Date:	2025/03/14	初回作成
			2025/03/15	ピッチングのシステム追加
						ピッチングカーソルの移動・描画
			2025/03/17	ストライク・ボール判定を反映
===================================================================+*/

// ==============================
//    インクルード部
// ==============================
#include "Pitching.h"
#include "Input.h"
#include "Collision.h"
#include "SceneGame.h"
#include "BallCount.h"

// ==============================
//    定数定義
// ==============================
constexpr float MAX_SPEED = 170.0f;
constexpr float SPEED_AJUST = 60.0f;

CPitching::CPitching()
	: m_pStrikeZone(nullptr), m_pCursor(nullptr)
	, m_nPitchingPhase((int)PitchingPhase::Set)
	, m_fSpeed(148.0f), m_fChatchTime(0.0f)
{

}

CPitching::~CPitching()
{
	m_pCursor.release();
	m_pStrikeZone.release();
}

void CPitching::Update()
{
	static float fPitchTime = 0.0f;	// ナイスピッチや着弾までに使うタイム
	static DirectX::XMFLOAT2 fCursorPos = { 0.0f,0.0f };	// ピッチカーソルの座標
	CBallCount* pBallCount = CBallCount::GetInstance().get();	// ボールカウントクラスのインスタンスを取得

	// ピッチング処理
	switch (m_nPitchingPhase)
	{
		// セットポジション
	case (int)CPitching::PitchingPhase::Set:
		// スペースキーで位置決定
		if (IsKeyTrigger(VK_SPACE))
		{
			fCursorPos = m_pCursor->GetPos((int)CSceneGame::Playing::Defence);
			fPitchTime = 0.0f;
			// 球速は乱数で一定値下がる可能性がある
			m_fSpeed = m_fSpeed - (float)(rand() % 3);
			// 投球する場所を決めたらフェーズを移す
			m_nPitchingPhase = (int)PitchingPhase::Pitch;
		}
		break;
		// リリースポイント
	case (int)CPitching::PitchingPhase::Pitch:
		fPitchTime += 1.0f / 60.0f;
		// リリースポイントのタイミングで投球の質を判断する
		if (IsKeyTrigger(VK_SPACE))
		{
			if (fPitchTime < 2.0f) INFO_MESSAGE("ミス");
			else if (fPitchTime < 2.5f)INFO_MESSAGE("ナイスピッチ");
			else if (fPitchTime < 2.7f)INFO_MESSAGE("ベストピッチ");
			else if (fPitchTime < 4.2f)INFO_MESSAGE("ナイスピッチ");
			else INFO_MESSAGE("ミス");

			// 投球したらフェーズを移す
			m_nPitchingPhase = (int)CPitching::PitchingPhase::Release;
			fPitchTime = 0.0f;
		}
		// 時間切れはミス投球になる
		else if(fPitchTime >= 4.2f)
		{
			INFO_MESSAGE("ミス");
			// 投球したらフェーズを移す
			m_nPitchingPhase = (int)CPitching::PitchingPhase::Release;
			fPitchTime = 0.0f;
		}
		// 球速に応じて捕球までの時間を決める
		m_fChatchTime = SPEED_AJUST / KMETER(m_fSpeed) * 60.0f * 60.0f;
		break;
		// 投球
	case (int)CPitching::PitchingPhase::Release:
		fPitchTime += 1.0f / 60.0f;

		// タイマーが捕球までの時間になったら
		if (fPitchTime >= m_fChatchTime)
		{
			// ストライクゾーンにカーソルのポジションが入っていればストライクのカウント
			// 入っていなければボールのカウントを増やす
			if (Collision::Hit2D(m_pStrikeZone->GetPos(), fCursorPos, m_pStrikeZone->GetSize(), { 0.0f,0.0f }))
			{
				INFO_MESSAGE("ストライク！");
				pBallCount->AddStrikeCount();
			}
			else
			{
				INFO_MESSAGE("ボール！");
				pBallCount->AddBallCount();
			}
			// セットポジションに戻る
			m_nPitchingPhase = (int)CPitching::PitchingPhase::Set;
		}
		break;
	default:
		break;
	}
}

void CPitching::Draw()
{
}

void CPitching::SetStrikeZone(CStrikeZone* zone)
{
	m_pStrikeZone.reset(zone);
}

void CPitching::SetCursor(CCursor* cursor)
{
	m_pCursor.reset(cursor);
}

/*+===================================================================
	File: Pitching.cpp
	Summary: 投球処理
	Author: 吉田京志郎
	Date:	2025/03/14	初回作成
			2025/03/15	ピッチングのシステム追加
						ピッチングカーソルの移動・描画
			2025/03/17	ストライク・ボール判定を反映
						ピッチングサークル、リリースポイントの描画
===================================================================+*/

// ==============================
//    インクルード部
// ==============================
#include "Pitching.h"
#include "Input.h"
#include "Collision.h"
#include "SceneGame.h"
#include "BallCount.h"
#include "Sprite.h"
#include "Camera.h"
#include "ImGuiManager.h"

// ==============================
//    定数定義
// ==============================
// 最高球速
constexpr float ce_fMax_Speed = 170.0f;
// 球速をこの数値で割ることで体感速度に補正
constexpr float ce_fSpeed_Ajust = 60.0f;
constexpr DirectX::XMFLOAT2 ce_fReleasePointSize = { 40.0f,40.0f };
constexpr DirectX::XMFLOAT2 ce_fPitchingCircleFirstSize = { 300.0f,300.0f };
constexpr DirectX::XMFLOAT2 ce_fPitchingCircleEndSize = { 20.0f,20.0f };
constexpr float ce_fSetPositionTime = 4.0f;
constexpr float ce_fCircleTime = 0.5f;

CPitching::CPitching()
	: m_pStrikeZone(nullptr), m_pPitchingCursor(nullptr)
	, m_nPitchingPhase((int)PitchingPhase::Set)
	, m_fSpeed(148.0f), m_fChatchTime(0.0f)
{
	for (int i = 0; i < (int)TexKind::Max; i++)
	{
		m_pTexture[i] = std::make_unique<Texture>();
	}

	m_pTexture[(int)TexKind::ReleasePoint]->Create(TEX_BALLSHEET);
	m_pTexture[(int)TexKind::PitchingCircle]->Create(TEX_PITCHINGCIRCLE);

	DirectX::XMFLOAT4X4 wvp[3];
	wvp[0] = CCamera::Get2DWolrdMatrix();
	wvp[1] = CCamera::Get2DViewMatrix();
	wvp[2] = CCamera::Get2DProjectionMatrix();

	m_tParam[(int)TexKind::ReleasePoint].color = {0.0f,0.0f,1.0f,0.5f};
	m_tParam[(int)TexKind::ReleasePoint].world = wvp[0];
	m_tParam[(int)TexKind::ReleasePoint].view = wvp[1];
	m_tParam[(int)TexKind::ReleasePoint].proj = wvp[2];
	m_tParam[(int)TexKind::ReleasePoint].uvPos = { 1.0f / (float)ce_nCountSplitX,2.0f / (float)ce_nCountSplitY };
	m_tParam[(int)TexKind::ReleasePoint].uvSize = { 1.0f / (float)ce_nCountSplitX,1.0f / (float)ce_nCountSplitY };
	m_tParam[(int)TexKind::PitchingCircle].size = ce_fPitchingCircleFirstSize;
	m_tParam[(int)TexKind::PitchingCircle].color = { 1.0f,1.0f,1.0f,0.5f };
	m_tParam[(int)TexKind::PitchingCircle].world = wvp[0];
	m_tParam[(int)TexKind::PitchingCircle].view = wvp[1];
	m_tParam[(int)TexKind::PitchingCircle].proj = wvp[2];
}

CPitching::~CPitching()
{
	m_pPitchingCursor.release();
	m_pStrikeZone.release();
}

void CPitching::Update()
{
	static float fPitchTime = 0.0f;	// ナイスピッチや着弾までに使うタイム
	static bool bSetCircle = false;	// ピッチングサークルを表示しているかどうか
	static DirectX::XMFLOAT2 fCursorPos = { 0.0f,0.0f };	// ピッチカーソルの座標
	CBallCount* pBallCount = CBallCount::GetInstance().get();	// ボールカウントクラスのインスタンスを取得

	// ピッチング処理
	switch (m_nPitchingPhase)
	{
		// セットポジション
	case (int)CPitching::PitchingPhase::Set:
		m_pPitchingCursor->SetMove(true);
		// スペースキーで位置決定
		if (IsKeyTrigger(VK_SPACE))
		{
			m_tParam[(int)TexKind::ReleasePoint].pos = m_tParam[(int)TexKind::PitchingCircle].pos = fCursorPos = m_pPitchingCursor->GetPos();
			fPitchTime = 0.0f;
			// 球速は乱数で一定値下がる可能性がある
			m_fSpeed = m_fSpeed - (float)(rand() % 3);
			// 最初はピッチングサークルを表示しない
			m_tParam[(int)TexKind::PitchingCircle].size = { 0.0f,0.0f };
			// 投球する場所を決めたらフェーズを移す
			m_nPitchingPhase = (int)PitchingPhase::Pitch;
			m_pPitchingCursor->SetMove(false);
		}
		break;
		// リリースポイント
	case (int)CPitching::PitchingPhase::Pitch:
		fPitchTime += 1.0f / 60.0f;

		// セットポジションから少し経つとピッチングサークルを表示する
		if (fPitchTime > ce_fSetPositionTime && !bSetCircle)
		{
			m_tParam[(int)TexKind::PitchingCircle].size = ce_fPitchingCircleFirstSize;
			bSetCircle = true;
		}

		if(bSetCircle)
		{
			// ピッチングサークルを縮小し、ベストピッチのタイミングでボールの大きさにする
			m_tParam[(int)TexKind::PitchingCircle].size.x -= (ce_fPitchingCircleFirstSize.x - ce_fPitchingCircleEndSize.x) / (ce_fCircleTime * fFPS);
			m_tParam[(int)TexKind::PitchingCircle].size.y -= (ce_fPitchingCircleFirstSize.y - ce_fPitchingCircleEndSize.y) / (ce_fCircleTime * fFPS);
		}

		// リリースポイントのタイミングで投球の質を判断する
		if (IsKeyTrigger(VK_SPACE))
		{
			DirectX::XMFLOAT2 fDefCursorPos = m_pPitchingCursor->GetPos();
			int randX = rand() % 20 - 10;
			int randY = rand() % 10 - 10;
			int randMiss = rand() % 10;

			if (m_tParam[(int)TexKind::PitchingCircle].size.x > ce_fReleasePointSize.x)
			{
				switch (randMiss)
				{
				case 0:
					m_pPitchingCursor->SetPos( {  0.0f,-100.0f  });
				default:
					m_pPitchingCursor->SetPos( { m_pStrikeZone->GetPos().x - m_pStrikeZone->GetSize().x / 1.3f,m_pStrikeZone->GetPos().y + m_pStrikeZone->GetSize().y / 1.3f });
					break;												   
				}
			}
			else if (m_tParam[(int)TexKind::PitchingCircle].size.x > ce_fPitchingCircleEndSize.x + 1.0f)
			{
				m_pPitchingCursor->SetPos( { fDefCursorPos.x + randX, fDefCursorPos.y + randY });
			}
			else if (m_tParam[(int)TexKind::PitchingCircle].size.x > ce_fPitchingCircleEndSize.x - 1.0f)
			{
				m_pPitchingCursor->SetPos( { fDefCursorPos.x,fDefCursorPos.y });
			}
			else if (m_tParam[(int)TexKind::PitchingCircle].size.x > ce_fPitchingCircleEndSize.x / 2.0f)
			{
				m_pPitchingCursor->SetPos( { fDefCursorPos.x + randX, fDefCursorPos.y + randY });
			}
			else
			{
				switch (randMiss)
				{
				case 0:
					m_pPitchingCursor->SetPos( { 0.0f,-100.0f });
				default:
					m_pPitchingCursor->SetPos( { m_pStrikeZone->GetPos().x - m_pStrikeZone->GetSize().x / 1.3f,m_pStrikeZone->GetPos().y + m_pStrikeZone->GetSize().y / 1.3f });
					break;
				}
			}

			// 投球したらフェーズを移す
			m_nPitchingPhase = (int)CPitching::PitchingPhase::Release;
			fPitchTime = 0.0f;
			fCursorPos = m_pPitchingCursor->GetPos();
		}
		else if(m_tParam[(int)TexKind::PitchingCircle].size.x < 0.0f)
		{
			int randMiss = rand() % 10;
			// 時間切れはミス投球になる
			switch (randMiss)
			{
			case 0:
				m_pPitchingCursor->SetPos( { 0.0f,-100.0f });
			default:
				m_pPitchingCursor->SetPos( { m_pStrikeZone->GetPos().x - m_pStrikeZone->GetSize().x / 1.3f,m_pStrikeZone->GetPos().y + m_pStrikeZone->GetSize().y / 1.3f });
				break;
			}
			// 投球したらフェーズを移す
			m_nPitchingPhase = (int)CPitching::PitchingPhase::Release;
			fPitchTime = 0.0f;
			fCursorPos = m_pPitchingCursor->GetPos();
		}
		// 球速に応じて捕球までの時間を決める
		m_fChatchTime = ce_fSpeed_Ajust / KMETER(m_fSpeed) * 60.0f * 60.0f;
		break;
		// 投球
	case (int)CPitching::PitchingPhase::Release:
		fPitchTime += 1.0f / 60.0f;
		bSetCircle = false;
		m_tParam[(int)TexKind::PitchingCircle].size = m_tParam[(int)TexKind::ReleasePoint].size = { 0.0f,0.0f };

		// タイマーが捕球までの時間になったら
		if (fPitchTime >= m_fChatchTime)
		{
			// バッターが見逃した時
			if (!CBatting::GetSwing())
			{
				// ストライクゾーンにカーソルのポジションが入っていればストライクのカウント
				// 入っていなければボールのカウントを増やす
				if (Collision::Hit2D(m_pPitchingCursor->GetCollision(true, Collision::eSquare), m_pStrikeZone->GetCollision()).isHit)
				{
					pBallCount->AddStrikeCount();
				}
				else
				{
					pBallCount->AddBallCount();
				}
			}

			// セットポジションに戻る
			m_nPitchingPhase = (int)CPitching::PitchingPhase::Set;
			m_pPitchingCursor->SetPos( { 0.0f,-100.0f });
		}
		break;
	default:
		break;
	}
}

void CPitching::Draw()
{
	DrawCircle();
}

void CPitching::SetStrikeZone(CStrikeZone* zone)
{
	m_pStrikeZone.reset(zone);
}

void CPitching::SetCursor(CPitchingCursor* cursor)
{
	m_pPitchingCursor.reset(cursor);
}

int CPitching::GetPitchingPhase()
{
	return m_nPitchingPhase;
}

float CPitching::GetChatchTime()
{
	return m_fChatchTime * fFPS;
}

void CPitching::DrawCircle()
{
#ifdef _IMGUI
	DirectX::XMFLOAT4 possize = GetPosSizeDebug("ReleasePoint");
#endif // _IMGUI

	m_tParam[(int)TexKind::ReleasePoint].size = ce_fReleasePointSize;
	if (m_nPitchingPhase == (int)CPitching::PitchingPhase::Pitch)
	{
		Sprite::SetParam(m_tParam[(int)TexKind::ReleasePoint]);
		Sprite::SetTexture(m_pTexture[(int)TexKind::ReleasePoint].get());
		Sprite::Draw();
		Sprite::SetParam(m_tParam[(int)TexKind::PitchingCircle]);
		Sprite::SetTexture(m_pTexture[(int)TexKind::PitchingCircle].get());
		Sprite::Draw();
	}
}

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
constexpr float ce_fMax_Speed = 170.0f;		// 最高球速
constexpr float ce_fSpeed_Ajust = 60.0f;	// 球速をこの数値で割ることで体感速度に補正
constexpr DirectX::XMFLOAT2 ce_fReleasePointSize = { 40.0f,40.0f };				// リリースタイミングのサイズ
constexpr DirectX::XMFLOAT2 ce_fPitchingCircleFirstSize = { 300.0f,300.0f };	// ピッチングサークルの最初のサイズ
constexpr DirectX::XMFLOAT2 ce_fPitchingCircleEndSize = { 20.0f,20.0f };		// ピッチングサークルの最後のサイズ
constexpr float ce_fSetPositionTime = 4.0f;	// セットポジションから投球までの時間(秒)
constexpr float ce_fCircleTime = 0.5f;		// ピッチングサークルが縮むまでの時間(秒)

CPitching::CPitching()
	: m_pStrikeZone(nullptr), m_pPitchingCursor(nullptr)
	, m_nPitchingPhase((int)PitchingPhase::Set)
	, m_fSpeed(148.0f), m_fChatchTime(0.0f)
{
	// テクスチャの読み込み
	for (int i = 0; i < (int)TexKind::Max; i++)
	{
		m_pTexture[i] = std::make_unique<Texture>();
	}
	m_pTexture[(int)TexKind::ReleasePoint]->Create(TEXPASS("BallCountSheet.png"));
	m_pTexture[(int)TexKind::PitchingCircle]->Create(TEXPASS("PitchingCircle.png"));

	// パラメータの初期化
	DirectX::XMFLOAT4X4 wvp[3];
	wvp[0] = CCamera::Get2DWolrdMatrix({0.0f,0.0f},0.0f);
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

	m_tPitcherState.m_bLeftPitcher = false;
	m_tPitcherState.m_fSpeed = 148.0f;
	m_tPitcherState.m_fStamina = 70.0f;
	m_tPitcherState.m_fControl = 70.0f;
	m_tPitcherState.m_nBenderQuality[(int)BenderKind::Fourseam] = 1;
	m_tPitcherState.m_nBenderQuality[(int)BenderKind::Twoseam] = 2;
	m_tPitcherState.m_nBenderQuality[(int)BenderKind::Slider] = 3;
	m_tPitcherState.m_nBenderQuality[(int)BenderKind::Curve] = 4;
	m_tPitcherState.m_nBenderQuality[(int)BenderKind::Split] = 5;
	m_tPitcherState.m_nBenderQuality[(int)BenderKind::Sinker] = 6;
	m_tPitcherState.m_nBenderQuality[(int)BenderKind::Shoot] = 7;
}

CPitching::~CPitching()
{
	// コンポジションインスタンスの放棄
	m_pPitchingCursor.release();
	m_pStrikeZone.release();
}

void CPitching::Update()
{
	static float fPitchTime = 0.0f;	// ナイスピッチや着弾までに使うタイム
	static bool bSetCircle = false;	// ピッチングサークルを表示しているかどうか
	CBallCount* pBallCount = CBallCount::GetInstance().get();	// ボールカウントクラスのインスタンスを取得
	CBallCount::Team eDefenceTeam = pBallCount->GetDefenseTeam();

	switch (CBall::GetInstance()->GetPhase())
	{
	case CBall::BallPhase::Batting:
		// ピッチング処理
		switch (m_nPitchingPhase)
		{
			// セットポジション
		case (int)CPitching::PitchingPhase::Set:
			// ストレート
			if (eDefenceTeam == CBallCount::Team::Player1 ? IsKeyTrigger(InputPlayer1::Up) : IsKeyTrigger(InputPlayer2::Up))
			{
				if (m_tPitcherState.m_nBenderQuality[(int)BenderKind::Fourseam] != 0) m_tPitcherState.m_eThrowKind = BenderKind::Fourseam;
			}
			// ツーシーム
			if (eDefenceTeam == CBallCount::Team::Player1 ? IsKeyTrigger(InputPlayer1::R1) : IsKeyTrigger(InputPlayer2::R1))
			{
				if (m_tPitcherState.m_nBenderQuality[(int)BenderKind::Twoseam] != 0 && m_tPitcherState.m_eThrowKind == BenderKind::Fourseam) m_tPitcherState.m_eThrowKind = BenderKind::Twoseam;
			}
			// スライダー
			if (eDefenceTeam == CBallCount::Team::Player1 ? IsKeyTrigger(InputPlayer1::Right) : IsKeyTrigger(InputPlayer2::Right))
			{
				if (m_tPitcherState.m_bLeftPitcher)
				{
					if (m_tPitcherState.m_nBenderQuality[(int)BenderKind::Shoot] != 0) m_tPitcherState.m_eThrowKind = BenderKind::Shoot;
				}
				else
				{
					if (m_tPitcherState.m_nBenderQuality[(int)BenderKind::Slider] != 0) m_tPitcherState.m_eThrowKind = BenderKind::Slider;
				}
			}
			// フォーク
			if (eDefenceTeam == CBallCount::Team::Player1 ? IsKeyTrigger(InputPlayer1::Down) : IsKeyTrigger(InputPlayer2::Down))
			{
				if (m_tPitcherState.m_nBenderQuality[(int)BenderKind::Split] != 0) m_tPitcherState.m_eThrowKind = BenderKind::Split;
			}
			// シュート
			if (eDefenceTeam == CBallCount::Team::Player1 ? IsKeyTrigger(InputPlayer1::Left) : IsKeyTrigger(InputPlayer2::Left))
			{
				if (m_tPitcherState.m_bLeftPitcher)
				{
					if (m_tPitcherState.m_nBenderQuality[(int)BenderKind::Slider] != 0) m_tPitcherState.m_eThrowKind = BenderKind::Slider;
				}
				else
				{
					if (m_tPitcherState.m_nBenderQuality[(int)BenderKind::Shoot] != 0) m_tPitcherState.m_eThrowKind = BenderKind::Shoot;
				}
			}
			// カーブ
			if ((eDefenceTeam == CBallCount::Team::Player1 ? IsKeyTrigger(InputPlayer1::Right) : IsKeyTrigger(InputPlayer2::Right)) && 
				(eDefenceTeam == CBallCount::Team::Player1 ? IsKeyTrigger(InputPlayer1::Down) : IsKeyTrigger(InputPlayer2::Down)))
			{
				if (m_tPitcherState.m_bLeftPitcher)
				{
					if (m_tPitcherState.m_nBenderQuality[(int)BenderKind::Sinker] != 0) m_tPitcherState.m_eThrowKind = BenderKind::Sinker;
				}
				else
				{
					if (m_tPitcherState.m_nBenderQuality[(int)BenderKind::Curve] != 0) m_tPitcherState.m_eThrowKind = BenderKind::Curve;
				}
			}
			// シンカー
			if ((eDefenceTeam == CBallCount::Team::Player1 ? IsKeyTrigger(InputPlayer1::Left) : IsKeyTrigger(InputPlayer2::Left)) &&
				(eDefenceTeam == CBallCount::Team::Player1 ? IsKeyTrigger(InputPlayer1::Down) : IsKeyTrigger(InputPlayer2::Down)))
			{
				if (m_tPitcherState.m_bLeftPitcher)
				{
					if (m_tPitcherState.m_nBenderQuality[(int)BenderKind::Curve] != 0) m_tPitcherState.m_eThrowKind = BenderKind::Curve;
				}
				else
				{
					if (m_tPitcherState.m_nBenderQuality[(int)BenderKind::Sinker] != 0) m_tPitcherState.m_eThrowKind = BenderKind::Sinker;
				}
			}

			// Aボタンで球種決定
			if (eDefenceTeam == CBallCount::Team::Player1 ? IsKeyTrigger(InputPlayer1::A) : IsKeyTrigger(InputPlayer2::A))
			{

				fPitchTime = 0.0f;
				// 球速は乱数で一定値下がる可能性がある
				m_fSpeed = m_fSpeed - (float)(rand() % 3);
				// 最初はピッチングサークルを表示しない
				m_tParam[(int)TexKind::PitchingCircle].size = { 0.0f,0.0f };
				// 球種を決めたらフェーズを移す
				m_nPitchingPhase = (int)PitchingPhase::Pitch;
			}
			break;
			// リリースポイント
		case (int)CPitching::PitchingPhase::Pitch:
			fPitchTime += 1.0f / 60.0f;
			m_pPitchingCursor->SetMove(true);
			m_tParam[(int)TexKind::ReleasePoint].pos = m_tParam[(int)TexKind::PitchingCircle].pos = m_pPitchingCursor->GetPos();

			// セットポジションから少し経ってからピッチングサークルを表示する
			if (fPitchTime > ce_fSetPositionTime && !bSetCircle)
			{
				m_tParam[(int)TexKind::PitchingCircle].size = ce_fPitchingCircleFirstSize;
				bSetCircle = true;
			}
			if (bSetCircle)
			{
				// ピッチングサークルを縮小し、ベストピッチのタイミングでボールの大きさにする
				m_tParam[(int)TexKind::PitchingCircle].size.x -= (ce_fPitchingCircleFirstSize.x - ce_fPitchingCircleEndSize.x) / (ce_fCircleTime * fFPS);
				m_tParam[(int)TexKind::PitchingCircle].size.y -= (ce_fPitchingCircleFirstSize.y - ce_fPitchingCircleEndSize.y) / (ce_fCircleTime * fFPS);
			}

			// リリースポイントのタイミングで投球の質を判断する
			if (eDefenceTeam == CBallCount::Team::Player1 ? IsKeyTrigger(InputPlayer1::A) : IsKeyTrigger(InputPlayer2::A))
			{
				DirectX::XMFLOAT2 fDefCursorPos = m_pPitchingCursor->GetPos();
				DirectX::XMFLOAT2 fDefPredPos = m_pPitchingCursor->GetPredPos();
				int randX = rand() % 20 - 10;
				int randY = rand() % 10 - 10;
				int randMiss = rand() % 10;

				// リリースが速い
				if (m_tParam[(int)TexKind::PitchingCircle].size.x > ce_fReleasePointSize.x)
				{
					switch (randMiss)
					{
					case 0:
						m_pPitchingCursor->SetPos(ce_fPitchingCursorPos);
						m_pPitchingCursor->SetPredPos(ce_fPitchingCursorPos);
						break;
					default:
						m_pPitchingCursor->SetPos({ m_pStrikeZone->GetPos().x - m_pStrikeZone->GetSize().x / 1.3f,m_pStrikeZone->GetPos().y + m_pStrikeZone->GetSize().y / 1.3f });
						m_pPitchingCursor->SetPredPos({ m_pStrikeZone->GetPos().x - m_pStrikeZone->GetSize().x / 1.3f,m_pStrikeZone->GetPos().y + m_pStrikeZone->GetSize().y / 1.3f });
						break;
					}
				}
				// リリースがやや速い
				else if (m_tParam[(int)TexKind::PitchingCircle].size.x > ce_fPitchingCircleEndSize.x + 1.0f)
				{
					m_pPitchingCursor->SetPos({ fDefCursorPos.x + randX, fDefCursorPos.y + randY });
					m_pPitchingCursor->SetPredPos({ fDefPredPos.x + randX, fDefPredPos.y + randY });
				}
				// ベストピッチ
				else if (m_tParam[(int)TexKind::PitchingCircle].size.x > ce_fPitchingCircleEndSize.x - 1.0f)
				{
					m_pPitchingCursor->SetPos({ fDefCursorPos.x,fDefCursorPos.y });
					m_pPitchingCursor->SetPredPos({ fDefPredPos.x, fDefPredPos.y });
				}
				// リリースがやや遅い
				else if (m_tParam[(int)TexKind::PitchingCircle].size.x > ce_fPitchingCircleEndSize.x / 2.0f)
				{
					m_pPitchingCursor->SetPos({ fDefCursorPos.x + randX, fDefCursorPos.y + randY });
					m_pPitchingCursor->SetPredPos({ fDefPredPos.x + randX, fDefPredPos.y + randY });
				}
				// リリースが遅い
				else
				{
					switch (randMiss)
					{
					case 0:
						m_pPitchingCursor->SetPos(ce_fPitchingCursorPos);
						m_pPitchingCursor->SetPredPos(ce_fPitchingCursorPos);
						break;
					default:
						m_pPitchingCursor->SetPos({ m_pStrikeZone->GetPos().x - m_pStrikeZone->GetSize().x / 1.3f,m_pStrikeZone->GetPos().y + m_pStrikeZone->GetSize().y / 1.3f });
						m_pPitchingCursor->SetPredPos({ m_pStrikeZone->GetPos().x - m_pStrikeZone->GetSize().x / 1.3f,m_pStrikeZone->GetPos().y + m_pStrikeZone->GetSize().y / 1.3f });
						break;
					}
				}

				// 投球したらボールをリリースする処理に移る
				m_nPitchingPhase = (int)CPitching::PitchingPhase::Release;
				fPitchTime = 0.0f;
			}
			else if (m_tParam[(int)TexKind::PitchingCircle].size.x < 0.0f)
			{
				int randMiss = rand() % 10;
				// 時間切れはミス投球になる
				switch (randMiss)
				{
				case 0:
					m_pPitchingCursor->SetPos(ce_fPitchingCursorPos);
					m_pPitchingCursor->SetPredPos(ce_fPitchingCursorPos);
					break;
				default:
					m_pPitchingCursor->SetPos({ m_pStrikeZone->GetPos().x - m_pStrikeZone->GetSize().x / 1.3f,m_pStrikeZone->GetPos().y + m_pStrikeZone->GetSize().y / 1.3f });
					m_pPitchingCursor->SetPredPos({ m_pStrikeZone->GetPos().x - m_pStrikeZone->GetSize().x / 1.3f,m_pStrikeZone->GetPos().y + m_pStrikeZone->GetSize().y / 1.3f });
					break;
				}
				// 投球したらボールをリリースする処理に移る
				m_nPitchingPhase = (int)CPitching::PitchingPhase::Release;
				fPitchTime = 0.0f;
			}
			// 球速に応じて捕球までの時間を決める
			m_fChatchTime = ce_fSpeed_Ajust / KMETER(m_fSpeed) * 60.0f * 60.0f;
			break;
			// 投球
		case (int)CPitching::PitchingPhase::Release:
			fPitchTime += 1.0f / 60.0f;
			bSetCircle = false;
			// リリースしたらリリースサークルの表示を消す
			m_tParam[(int)TexKind::PitchingCircle].size = m_tParam[(int)TexKind::ReleasePoint].size = { 0.0f,0.0f };
			m_pPitchingCursor->SetMove(false);

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
				// 振った時の処理はBatting.cppに記述する

				// セットポジションに戻る
				m_nPitchingPhase = (int)CPitching::PitchingPhase::Set;
				// カーソルの位置をリセットする
				m_pPitchingCursor->SetPos(ce_fPitchingCursorPos);
			}
			break;
		default:
			break;
		}
		break;
	case CBall::BallPhase::InPlay:
		m_nPitchingPhase = (int)CPitching::PitchingPhase::Set;
		m_pPitchingCursor->SetPos(ce_fPitchingCursorPos);
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

CPitching::PitchingPhase CPitching::GetPitchingPhase()
{
	return (PitchingPhase)m_nPitchingPhase;
}

float CPitching::GetChatchTime()
{
	return m_fChatchTime;
}

float CPitching::GetBendStartTime()
{
	return m_fChatchTime * 0.5f;
}

CPitching::PitchState CPitching::GetPitchState()
{
	return m_tPitcherState;
}

void CPitching::DrawCircle()
{
#ifdef _IMGUI
	DirectX::XMFLOAT4 possize = GetPosSizeDebug("ReleasePoint");
#endif // _IMGUI

	m_tParam[(int)TexKind::ReleasePoint].size = ce_fReleasePointSize;
	// ピッチング時に描画する
	if (m_nPitchingPhase == (int)CPitching::PitchingPhase::Pitch)
	{
		for (int i = 0; i < (int)TexKind::Max; i++)
		{
			m_tParam[i].world = CCamera::Get2DWolrdMatrix(m_tParam[i].pos, m_tParam[i].rotate);
			Sprite::SetParam(m_tParam[i]);
			Sprite::SetTexture(m_pTexture[i].get());
			Sprite::Draw();
		}
	}
}

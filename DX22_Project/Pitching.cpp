#include "Pitching.h"
#include "Input.h"
#include "Collision.h"
#include "GameManager.h"
#include "Main.h"
#include "PitcherData.h"

constexpr float ce_fMax_Speed = 170.0f;		// 最高球速
constexpr float ce_fSpeed_Adjust = 40.0f;	// 球速をこの数値で割ることで体感速度に補正
constexpr DirectX::XMFLOAT2 ce_fReleasePointSize = { 30.0f,30.0f };				// リリースタイミングのサイズ
constexpr float ce_fSetPositionTime = 4.0f;	// セットポジションから投球までの時間(秒)
constexpr float ce_fMinControl = 30.0f;

// コントロールメモ
// 60.65 S
// 40.60 A
// 40.55 B
// 40.50 C
// 40.45 D
// 40.40 E
// 35.35 F
// 30.30 G


CPitching::CPitching()
	: m_nPitchingPhase((int)PitchingPhase::Set)
	, m_fSpeed(148.0f), m_fChatchTime(0.0f)
	, m_pPitchingCircle(nullptr), m_pReleasePoint(nullptr)
{
	m_tPitcherState.m_bLeftPitcher = false;
	m_tPitcherState.m_fSpeed = 148.0f;
	m_tPitcherState.m_fStamina = 70.0f;
	m_tPitcherState.m_fControl = 70.0f;
	m_tPitcherState.m_eThrowKind = BenderKind::Fourseam;
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

}

void CPitching::Init()
{
	CScene* pScene = GetScene();

	m_pReleasePoint = pScene->AddGameObject<CReleasePoint>("ReleasePoint", Tag::UI);
	m_pPitchingCircle = pScene->AddGameObject<CPitchingCircle>("PitchingCircle", Tag::UI);
}

void CPitching::Update(int DefenceTeam)
{
	static float fPitchTime = 0.0f;	// ナイスピッチや着弾までに使うタイム
	static bool bSetCircle = false;	// ピッチングサークルを表示しているかどうか
	CScene* pScene = GetScene();
	CGameManager* pGameManager = CGameManager::GetInstance();	// ボールカウントクラスのインスタンスを取得
	CTeamDirector* pTeamDirecter = pGameManager->GetTeamDirecter(DefenceTeam);
	CPitchingCursor* pPitchingCursor = pScene->GetGameObject<CPitchingCursor>();
	CStrikeZone* pStrikeZone = pScene->GetGameObject<CStrikeZone>();
	CBatting* pBatting = pGameManager->GetAttackDirecter()->GetBatting();

	switch (pGameManager->GetPhase())
	{
	case GamePhase::Batting:
		// ピッチング処理
		switch (m_nPitchingPhase)
		{
			// セットポジション
		case (int)CPitching::PitchingPhase::Set:
			// ストレート
			if (IsKeyTrigger(DefenceTeam, Input::Up))
			{
				m_fSpeed = pTeamDirecter->GetTeam()->GetTakingPitcher()->GetPitcherData().m_fSpeed;
				if (m_tPitcherState.m_nBenderQuality[(int)BenderKind::Fourseam] != 0) m_tPitcherState.m_eThrowKind = BenderKind::Fourseam;
			}
			// ツーシーム
			if (IsKeyTrigger(DefenceTeam, Input::R1))
			{
				m_fSpeed = pTeamDirecter->GetTeam()->GetTakingPitcher()->GetPitcherData().m_fSpeed - 2;
				if (m_tPitcherState.m_nBenderQuality[(int)BenderKind::Twoseam] != 0 && m_tPitcherState.m_eThrowKind == BenderKind::Fourseam) m_tPitcherState.m_eThrowKind = BenderKind::Twoseam;
			}
			// スライダー
			if (IsKeyTrigger(DefenceTeam, Input::Right))
			{
				m_fSpeed = pTeamDirecter->GetTeam()->GetTakingPitcher()->GetPitcherData().m_fSpeed - 10;
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
			if (IsKeyTrigger(DefenceTeam, Input::Down))
			{
				m_fSpeed = pTeamDirecter->GetTeam()->GetTakingPitcher()->GetPitcherData().m_fSpeed - 12;
				if (m_tPitcherState.m_nBenderQuality[(int)BenderKind::Split] != 0) m_tPitcherState.m_eThrowKind = BenderKind::Split;
			}
			// シュート
			if (IsKeyTrigger(DefenceTeam, Input::Left))
			{
				m_fSpeed = pTeamDirecter->GetTeam()->GetTakingPitcher()->GetPitcherData().m_fSpeed - 5;
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
			if (IsKeyTrigger(DefenceTeam, Input::Right) && IsKeyTrigger(DefenceTeam, Input::Down))
			{
				m_fSpeed = pTeamDirecter->GetTeam()->GetTakingPitcher()->GetPitcherData().m_fSpeed - 15;
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
			if (IsKeyTrigger(DefenceTeam, Input::Left) && IsKeyTrigger(DefenceTeam, Input::Down))
			{
				m_fSpeed = pTeamDirecter->GetTeam()->GetTakingPitcher()->GetPitcherData().m_fSpeed - 7;
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
			if (IsKeyTrigger(DefenceTeam, Input::A))
			{

				fPitchTime = 0.0f;

				// 球速は乱数で一定値下がる可能性がある
				m_fSpeed = m_fSpeed - (float)(rand() % 3);
				// 最初はピッチングサークルを表示しない
				m_pPitchingCircle->SetSize({0.0f, 0.0f,0.0f });
				// 球種を決めたらフェーズを移す
				m_nPitchingPhase = (int)PitchingPhase::Pitch;
				float nControl = int(pTeamDirecter->GetTeam()->GetTakingPitcher()->GetPitcherData().m_eControl) * 5 + ce_fMinControl;
				m_pReleasePoint->SetSize({nControl, nControl, 0.0f});
			}
			break;
			// リリースポイント
		case (int)CPitching::PitchingPhase::Pitch:
			fPitchTime += 1.0f / 60.0f;
			pPitchingCursor->SetMove(true);
			m_pReleasePoint->SetFuturePos(pPitchingCursor->GetBallPos());
			m_pPitchingCircle->SetFuturePos(pPitchingCursor->GetBallPos());

			// セットポジションから少し経ってからピッチングサークルを表示する
			if (fPitchTime > ce_fSetPositionTime && !bSetCircle)
			{
				m_pPitchingCircle->ResetInitSize();
				bSetCircle = true;
			}
			if (bSetCircle)
			{
				// ピッチングサークルを縮小し、ベストピッチのタイミングでボールの大きさにする
				m_pPitchingCircle->StartScaleDown();
			}

			DirectX::XMFLOAT3 pitchingCircleSize = m_pPitchingCircle->GetSize();
			// リリースポイントのタイミングで投球の質を判断する
			if (IsKeyTrigger(DefenceTeam, Input::A))
			{
				DirectX::XMFLOAT3 fDefCursorPos = pPitchingCursor->GetBallPos();
				DirectX::XMFLOAT3 fDefPredPos = pPitchingCursor->GetPredPos();
				int randX = rand() % 20 - 10;
				int randY = rand() % 10 - 10;
				int randMiss = rand() % 10;
				DirectX::XMFLOAT3 releasePointSize = m_pReleasePoint->GetSize();


				m_fChatchTime = ce_fSpeed_Adjust / KMETER(m_fSpeed) * 60.0f * 60.0f;
				// リリースが速い
				if (pitchingCircleSize.x > releasePointSize.x)
				{
					switch (randMiss)
					{
					case 0:
						pPitchingCursor->SetBallPos(ce_fPitchingCursorPos);
						pPitchingCursor->SetPredPos(ce_fPitchingCursorPos);
						break;
					default:
						DirectX::XMFLOAT3 f3StrikeZonePos = pStrikeZone->GetPos();
						DirectX::XMFLOAT3 f3StrikeZoneSize = pStrikeZone->GetSize();
						pPitchingCursor->SetBallPos({ f3StrikeZonePos.x - f3StrikeZoneSize.x / 1.3f,f3StrikeZonePos.y - f3StrikeZoneSize.y / 1.3f,0.0f });
						pPitchingCursor->SetPredPos({ f3StrikeZonePos.x - f3StrikeZoneSize.x / 1.3f,f3StrikeZonePos.y - f3StrikeZoneSize.y / 1.3f,0.0f });
						break;
					}
				}
				// リリースがやや速い
				else if (pitchingCircleSize.x > ce_fPitchingCircleEndSize.x + 1.0f)
				{
					pPitchingCursor->SetBallPos({ fDefCursorPos.x + randX, fDefCursorPos.y + randY,0.0f });
					pPitchingCursor->SetPredPos({ fDefPredPos.x + randX, fDefPredPos.y + randY,0.0f });
				}
				// ベストピッチ
				else if (pitchingCircleSize.x > ce_fPitchingCircleEndSize.x - 1.0f)
				{
					pPitchingCursor->SetBallPos({ fDefCursorPos.x,fDefCursorPos.y,0.0f });
					pPitchingCursor->SetPredPos({ fDefPredPos.x, fDefPredPos.y,0.0f });
				}
				// リリースがやや遅い
				else if (pitchingCircleSize.x > ce_fPitchingCircleEndSize.x / 2.0f)
				{
					pPitchingCursor->SetBallPos({ fDefCursorPos.x + randX, fDefCursorPos.y + randY,0.0f });
					pPitchingCursor->SetPredPos({ fDefPredPos.x + randX, fDefPredPos.y + randY,0.0f });
				}
				// リリースが遅い
				else
				{
					switch (randMiss)
					{
					case 0:
						pPitchingCursor->SetBallPos(ce_fPitchingCursorPos);
						pPitchingCursor->SetPredPos(ce_fPitchingCursorPos);
						break;
					default:
						DirectX::XMFLOAT3 f3StrikeZonePos = pStrikeZone->GetPos();
						DirectX::XMFLOAT3 f3StrikeZoneSize = pStrikeZone->GetSize();
						pPitchingCursor->SetBallPos({ f3StrikeZonePos.x - f3StrikeZoneSize.x / 1.3f,f3StrikeZonePos.y - f3StrikeZoneSize.y / 1.3f,0.0f });
						pPitchingCursor->SetPredPos({ f3StrikeZonePos.x - f3StrikeZoneSize.x / 1.3f,f3StrikeZonePos.y - f3StrikeZoneSize.y / 1.3f,0.0f });
						break;
					}
				}

				// 投球したらボールをリリースする処理に移る
				CBall* pBall = pScene->GetGameObject<CBall>();
				auto SEList = pBall->GetSEList();
				SEList["Pitching"]->Play();
				m_pPitchingCircle->Pitched();
				m_nPitchingPhase = (int)CPitching::PitchingPhase::Release;
				pBall->SetPitching(m_fChatchTime,m_tPitcherState.m_eThrowKind);
				fPitchTime = 0.0f;
			}
			else if (pitchingCircleSize.x < 0.0f)
			{

				int randMiss = rand() % 10;
				// 時間切れはミス投球になる
				switch (randMiss)
				{
				case 0:
					pPitchingCursor->SetBallPos(ce_fPitchingCursorPos);
					pPitchingCursor->SetPredPos(ce_fPitchingCursorPos);
					break;
				default:

					DirectX::XMFLOAT3 f3StrikeZonePos = pStrikeZone->GetPos();
					DirectX::XMFLOAT3 f3StrikeZoneSize = pStrikeZone->GetSize();
					pPitchingCursor->SetBallPos({ f3StrikeZonePos.x - f3StrikeZoneSize.x / 1.3f,f3StrikeZonePos.y - f3StrikeZoneSize.y / 1.3f,0.0f });
					pPitchingCursor->SetPredPos({ f3StrikeZonePos.x - f3StrikeZoneSize.x / 1.3f, f3StrikeZonePos.y - f3StrikeZoneSize.y / 1.3f,0.0f });
					break;
				}
				// 投球したらボールをリリースする処理に移る
				m_pPitchingCircle->Pitched();
				m_nPitchingPhase = (int)CPitching::PitchingPhase::Release;

				CBall* pBall = pScene->GetGameObject<CBall>();
				auto SEList = pBall->GetSEList();
				SEList["Pitching"]->Play();
				pBall->SetPitching(m_fChatchTime, m_tPitcherState.m_eThrowKind);
				fPitchTime = 0.0f;
			}
			// 球速に応じて捕球までの時間を決める
			break;
			// 投球
		case (int)CPitching::PitchingPhase::Release:
			fPitchTime += 1.0f / 60.0f;
			bSetCircle = false;
			// リリースしたらリリースサークルの表示を消す
			m_pPitchingCircle->SetSize({ 0.0f,0.0f,0.0f });
			m_pReleasePoint->SetSize({ 0.0f,0.0f,0.0f });
			pPitchingCursor->SetMove(false);

			// タイマーが捕球までの時間になったら
			if (fPitchTime >= m_fChatchTime)
			{
				// バッターが見逃した時
				if (!pBatting->GetSwing())
				{
					// ストライクゾーンにカーソルのポジションが入っていればストライクのカウント
					// 入っていなければボールのカウントを増やす
					Collision::Info2D  cursor;
					cursor.square.pos = pPitchingCursor->GetPos();
					cursor.square.size = DirectX::XMFLOAT3();
					cursor.type = Collision::eSquare;
					if (Collision::Hit2D(cursor, pStrikeZone->GetCollision()).isHit)
					{
						pGameManager->GetCountDirecter()->AddStrikeCount();
					}
					else
					{
						pGameManager->GetCountDirecter()->AddBallCount();
					}
				}
				// 振った時の処理はBatting.cppに記述する

				// セットポジションに戻る
				m_nPitchingPhase = (int)CPitching::PitchingPhase::Set;
				// カーソルの位置をリセットする
				pPitchingCursor->SetBallPos(ce_fPitchingCursorPos);
			}
			break;
		default:
			break;
		}
		break;
	case GamePhase::InPlay:
		m_nPitchingPhase = (int)CPitching::PitchingPhase::Set;
		pPitchingCursor->SetBallPos(ce_fPitchingCursorPos);
		break;
	default:
		break;
	}
}

void CPitching::Draw()
{
	DrawCircle();
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
//	int nControl = CTeamManager::GetInstance((int)CGameManager::GetInstance()->GetDefenseTeam())->GetTakingPitcherState().m_eControl;
//	releasePointSize.size = { (35.0f / 7.0f) * nControl + 30.0f,(35.0f / 7.0f) * nControl + 30.0f };
//	// ピッチング時に描画する
//	if (m_nPitchingPhase == (int)CPitching::PitchingPhase::Pitch)
//	{
//		for (int i = 0; i < (int)TexKind::Max; i++)
//		{
//			m_tParam[i].world = CCamera::Get2DWolrdMatrix(m_tParam[i].pos, m_tParam[i].rotate);
//			Sprite::SetParam(m_tParam[i]);
//			Sprite::SetTexture(m_pTexture[i].get());
//			Sprite::Draw();
//		}
//	}
}

#include "Batting.h"
#include "Input.h"
#include "BallCount.h"
#include "PitchingCursor.h"
#include "Main.h"
#include "GameManager.h"
#include "FielderData.h"

constexpr float ce_fJustTyming = 139.0f;	// ジャストミートになるタイミング
constexpr float ce_fHittingTyming = 4.0f;	// バットに当たれるタイミング(+-)
constexpr float ce_fAngleMax = 60.0f;		// x方向打球角度の限界(+-)

// ==============================
//    メモ
// ==============================
// Power4.5f...パワーD
// Power2.5f...パワーG
// 
// Angle40.0f...弾道4
// Angle30.0f...弾道3
// Angle20.0f...弾道2
// Angle10.0f...弾道1


CBatting::CBatting()
	: m_bBatting(false), m_fMoveDirection{}
	, m_fPower(0.0f), m_nTakingBatterNo{1,1}
{

}

CBatting::~CBatting()
{

}

void CBatting::Update(int AttackPlayer)
{
	DirectX::XMFLOAT3 fBallPos = GetScene()->GetGameObject<CBall>()->GetPos();
	CGameManager* pGameManager = CGameManager::GetInstance();
	CTeamDirector* pTeamDirector = pGameManager->GetTeamManager(AttackPlayer);
	CScene* pScene = GetScene();

	// ピッチャーがボールを受け取ったらスイング可能にする
	if (fBallPos.z == ce_fBallPos.z + WORLD_AJUST && CGameManager::GetInstance()->GetPhase() == GamePhase::Batting)
	{
		m_bSwing = false;
		m_bBatting = false;
	}

	// 投球中にスイングを掛けていない時にスイングが出来る
	if (IsKeyTrigger(AttackPlayer,Input::A) && !m_bSwing)
	{
		float fTyming = ce_fJustTyming + WORLD_AJUST - fBallPos.z;	// どのタイミングで振ったか(0がジャスト、マイナスが遅れている、プラスが早い)
		float fAngle = 30.0f;		// 打球角度
		float fDirection = 0.0f;	// 打球方向
		float fShotPower = 0.0f;	// ショットの強さ
		float fAngleDef = 0.0;
		
		do
		{ 
			// 早すぎるスイングはスイングとして扱わない
			if (fTyming > 50.0f) break;

			FielderData tTakingBatterData = pTeamDirector->GetTeam()->GetTakingBatter(m_nTakingBatterNo[AttackPlayer - 1])->GetFielderData();
			switch (tTakingBatterData.m_ePower)
			{
			case Quality::S: m_fPower = 6.0f; break;
			case Quality::A: m_fPower = 5.5f; break;
			case Quality::B: m_fPower = 5.0f; break;
			case Quality::C: m_fPower = 4.5f; break;
			case Quality::D: m_fPower = 4.0f; break;
			case Quality::E: m_fPower = 3.5f; break;
			case Quality::F: m_fPower = 3.0f; break;
			case Quality::G: m_fPower = 2.5f; break;
			default:
				break;
			}

			switch (tTakingBatterData.m_nTrajectory)
			{
			case 1: fAngleDef = 10.0f; break;
			case 2: fAngleDef = 20.0f; break;
			case 3: fAngleDef = 30.0f; break;
			case 4: fAngleDef = 40.0f; break;
			default:
				break;
			}
			
			// スイングした
     		m_bSwing = true;

			// タイミングチェック 
			if (fTyming > ce_fHittingTyming || fTyming < -ce_fHittingTyming)
			{
				pGameManager->GetCountManager()->AddStrikeCount();
				break;
			}
			else
			{
				Collision::Circle batting, pitching;
				CBattingCursor* pBattingCursor = pScene->GetGameObject<CBattingCursor>();
				CPitchingCursor* pPitchingCursor = pScene->GetGameObject<CPitchingCursor>();
				batting.pos = pBattingCursor->GetPos();
				batting.radius = pBattingCursor->GetSize().x;
				pitching.pos = pPitchingCursor->GetPos();
				pitching.radius = pPitchingCursor->GetSize().x;

				Collision::Result2D result = Collision::Hit2D(pitching,batting);
				// バットに当たった
				if(result.isHit)
				{
					// ミートカーソルの中央がボールの中央からどれだけ離れているか線形補間で求める
					DirectX::XMFLOAT2 fDistanceRatio = { result.posAtoB.x / result.distance.x * 100.0f ,result.posAtoB.y / result.distance.y * 100.0f };
					DirectX::XMFLOAT2 fSlowDown = {};

					// 打球速度決定
					if (fabsf(fDistanceRatio.x) >= 20.0f)fSlowDown.x = (100 - fabsf(fDistanceRatio.x)) / 150.0f;
					else if (fabsf(fDistanceRatio.x) >= 10.0f)fSlowDown.x = (100 - fabsf(fDistanceRatio.x)) / 150.0f;
					if (fabsf(fDistanceRatio.y) >= 20.0f)fSlowDown.y = (100 - fabsf(fDistanceRatio.y)) / 150.0f;
					else if (fabsf(fDistanceRatio.y) >= 10.0f)fSlowDown.y = (100 - fabsf(fDistanceRatio.y)) / 150.0f;

					// 減速を元のパワーと掛け合わせて打球の強さを求める
					// 減速がない場合元の強さをそのまま計算する
					fShotPower = m_fPower * (fSlowDown.x == 0.0f ? 1 : fSlowDown.x * fSlowDown.y == 0.0f ? 1 : fSlowDown.y);

					// 捉えた場所が端すぎるならファールチップとしてストライクにする
					if (fabsf(fDistanceRatio.x) >= 75.0f || fabsf(fDistanceRatio.y) >= 75.0f)
					{
						pGameManager->GetCountManager()->AddStrikeCount();
						break;
					}

					// 打球方向決定
					fDirection = fTyming * (ce_fAngleMax / ce_fHittingTyming);
					fDirection = fDirection - ce_fAngleMax / 2.0f * fDistanceRatio.x / 100.0f;
					fDirection = DirectX::XMConvertToRadians(fDirection);

					// 打球角度決定
					if (fDistanceRatio.y > 0)fDistanceRatio.y *= 3.0f;
					else fDistanceRatio.y /= 2.0f;
					fAngle = fAngleDef - ce_fAngleMax / 2.0f * fDistanceRatio.y / 100.0f;
					fAngle = DirectX::XMConvertToRadians(fAngle);
					
					// 方向と角度から打球の進行方向を決める
					// x計算のsinfに-を付けると左バッターになる
					m_fMoveDirection.x = cosf(fAngle) * sinf(fDirection);
					m_fMoveDirection.y = sinf(fAngle);                    
					m_fMoveDirection.z = -cosf(fAngle) * fabs(cosf(fDirection));

					// 進行方向ベクトルを正規化し、スケーリングすることで打球の強さを決める
					DirectX::XMVECTOR vecMove = DirectX::XMLoadFloat3(&m_fMoveDirection);
					vecMove = DirectX::XMVector3Normalize(vecMove);
					vecMove = DirectX::XMVectorScale(vecMove, fShotPower);
					DirectX::XMStoreFloat3(&m_fMoveDirection, vecMove);

					// バットに当たった
					m_bBatting = true;
					break;
				}
				else
				{
					// 空振り
					pGameManager->GetCountManager()->AddStrikeCount();
				}
			}
		} while (0);
	}
}

void CBatting::Draw()
{

}

DirectX::XMFLOAT3 CBatting::GetDirection()
{
	return m_fMoveDirection;
}

bool CBatting::GetBatting()
{
	return m_bBatting;
}

void CBatting::CheckHit()
{
}

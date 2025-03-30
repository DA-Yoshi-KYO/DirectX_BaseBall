#include "Batting.h"
#include "Input.h"
#include "BallCount.h"
#include "Collision.h"
#include "PitchingCursor.h"

constexpr float ce_fJustTyming = 134.0f;
constexpr float ce_fHittingTyming = 4.0f;
constexpr float ce_fAngleMax = 60.0f;
constexpr float ce_fHomeRunAngle = 30.0f;

bool CBatting::m_bSwing = false;

CBatting::CBatting()
	: m_pBattingCursor(nullptr), m_pBall(nullptr)
	, m_bBatting(false), m_fMoveDirection{}
	, m_fPower(4.5f)
{

}

CBatting::~CBatting()
{
	m_pBattingCursor.release();
	m_pBall.release();
}

void CBatting::Update()
{
	DirectX::XMFLOAT3 fBallPos = m_pBall->GetPos();
	CBallCount* pBallCount = CBallCount::GetInstance().get();	// ボールカウントクラスのインスタンスを取得

	if (fBallPos.z == ce_fBallPos.z + WORLD_AJUST)
	{
		// ピッチャーがボールを受け取ったらスイング可能にする
		m_bSwing = false;
	}

	// 投球中にスイングを掛けていない時にスイングが出来る
	if (pBallCount->GetOffenseTeam() == CBallCount::Team::Player1 ? IsKeyPress(InputPlayer1::A) : IsKeyPress(InputPlayer2::A) && !m_bSwing && !m_bBatting)
	{
		float fTyming = ce_fJustTyming + WORLD_AJUST - fBallPos.z;	// どのタイミングで振ったか(0がジャスト、マイナスが遅れている、プラスが早い)
		float fAngle = 30.0f;		// 打球角度
		float fDirection = 0.0f;	// 打球方向
		float fShotPower = 0.0f;			// ショットの強さ

		do
		{ 
			// 早すぎるスイングはスイングとして扱わない
			if (fTyming > 50.0f) break;
			
			// スイングした
			m_bSwing = true;

			// タイミングチェック 
			if (fTyming > ce_fHittingTyming || fTyming < -ce_fHittingTyming)
			{
				pBallCount->AddStrikeCount();
				break;
			}
			else
			{
				Collision::Result2D result = Collision::Hit2D(m_pBattingCursor->GetCollision(false), CPitchingCursor::GetCollision(false));
				if(result.isHit)
				{
					// ミートカーソルの中央がボールの中央からどれだけ離れているか線形補間で求める
					DirectX::XMFLOAT2 fDistanceRatio = { result.posAtoB.x / result.distance.x * 100.0f ,result.posAtoB.y / result.distance.y * 100.0f };
					DirectX::XMFLOAT2 fSlowDown = {};

					// 打球速度決定
					if (fabsf(fDistanceRatio.x) >= 10.0f)fSlowDown.x = (100 - fabsf(fDistanceRatio.x)) / 150.0f;
					if (fabsf(fDistanceRatio.y) >= 10.0f)fSlowDown.y = (100 - fabsf(fDistanceRatio.y)) / 150.0f;

					// 減速を元のパワーと掛け合わせて打球の強さを求める
					// 減速がない場合元の強さをそのまま計算する
					fShotPower = m_fPower * (fSlowDown.x == 0.0f ? 1 : fSlowDown.x * fSlowDown.y == 0.0f ? 1 : fSlowDown.y);

					// 捉えた場所が端すぎるならファールチップとしてストライクにする
					if (fabsf(fDistanceRatio.x) >= 75.0f || fabsf(fDistanceRatio.y) >= 75.0f)
					{
						pBallCount->AddStrikeCount();
						break;
					}

					// 打球方向決定
					fDirection = fTyming * (ce_fAngleMax / ce_fHittingTyming);
					fDirection = fDirection - ce_fAngleMax / 2.0f * fDistanceRatio.x / 100.0f;
					fDirection = DirectX::XMConvertToRadians(fDirection);

					// 打球角度決定
					if (fDistanceRatio.y > 0)fDistanceRatio.y *= 3.0f;
					else fDistanceRatio.y /= 2.0f;
					fAngle = ce_fHomeRunAngle - ce_fAngleMax / 2.0f * fDistanceRatio.y / 100.0f;
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
					pBallCount->AddStrikeCount();
				}
			}
		} while (0);
	}
}

void CBatting::Draw()
{

}

void CBatting::SetCursor(CBattingCursor* cursor)
{
	m_pBattingCursor.reset(cursor);
}

void CBatting::SetBall(CBall* ball)
{
	m_pBall.reset(ball);
}

DirectX::XMFLOAT3 CBatting::GetDirection()
{
	return m_fMoveDirection;
}

bool CBatting::GetSwing()
{
	return m_bSwing;
}

bool CBatting::GetBatting()
{
	return m_bBatting;
}

void CBatting::SetBatting(bool isBatting)
{
	m_bBatting = isBatting;
}

#include "Batting.h"
#include "Input.h"
#include "BallCount.h"
#include "Collision.h"
#include "PitchingCursor.h"

constexpr float ce_fJustTyming = 138.0f;
constexpr float ce_fHittingTyming = 5.0f;
constexpr float ce_fAngleMax = 60.0f;

CBatting::CBatting()
	: m_pBattingCursor(nullptr), m_pBall(nullptr)
	, m_bSwing(false), m_fMoveDirection{}
	, m_fPower(5.0f), m_bBatting(false)
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

	if (fBallPos.z == ce_fBallPos.z + WORLD_AJUST) m_bSwing = false;

	if (IsKeyTrigger(VK_RETURN) && !m_bSwing && !m_bBatting)
	{
		float fTyming = ce_fJustTyming + WORLD_AJUST - fBallPos.z;
		float fAngle = 30.0f;
		float fDirection = 0.0f;
		std::string debugST;

		do
		{ 
			if (fTyming > 50.0f) break;
			
			// タイミングチェック 
			if (fTyming > ce_fHittingTyming || fTyming < -ce_fHittingTyming)
			{
				INFO_MESSAGE("空振り");
				m_bSwing = true;
				break;
			}
			else
			{
				fDirection = fTyming * (ce_fAngleMax / ce_fHittingTyming);

				if(Collision::Hit2D(m_pBattingCursor->GetCollision(false), CPitchingCursor::GetCollision(false)).isHit)
				{
					DirectX::XMFLOAT3 f3Angle;
					fAngle = DirectX::XMConvertToRadians(fAngle);
					fDirection = DirectX::XMConvertToRadians(fDirection);

					// sinfに-を付けると左バッターになる
					f3Angle.x = cosf(fAngle) * sinf(fDirection);

					f3Angle.y = sinf(fAngle);                    
					f3Angle.z = -cosf(fAngle) * fabs(cosf(fDirection));

					DirectX::XMVECTOR vecMove = DirectX::XMLoadFloat3(&f3Angle);
					vecMove = DirectX::XMVector3Normalize(vecMove);
					vecMove = DirectX::XMVectorScale(vecMove, m_fPower);
					DirectX::XMStoreFloat3(&m_fMoveDirection, vecMove);

					m_bBatting = true;
				}
				else
				{

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

bool CBatting::GetBatting()
{
	return m_bBatting;
}

void CBatting::SetBatting(bool isBatting)
{
	m_bBatting = isBatting;
}

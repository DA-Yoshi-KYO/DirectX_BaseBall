#include "Batting.h"
#include "Input.h"
#include "BallCount.h"

constexpr float ce_fJustTyming = 135.0f;

CBatting::CBatting()
	: m_pCursor(nullptr), m_pBall(nullptr)
	, m_bSwing(false)
{

}

CBatting::~CBatting()
{
	m_pCursor.release();
}

void CBatting::Update()
{
	DirectX::XMFLOAT3 fBallPos = m_pBall->GetPos();

	if (fBallPos.z == ce_fBallPos.z) m_bSwing = true;

	if (IsKeyTrigger(VK_RETURN) && !m_bSwing)
	{
		float fTyming = ce_fJustTyming - fBallPos.z;

		do
		{
			if (fTyming > 50.0f) break;

			if (fTyming > 5.0f)
			{
				m_bSwing = true;
				break;
			}

		} while (0);
	}
}

void CBatting::Draw()
{

}

void CBatting::SetCursor(CCursor* cursor)
{
	m_pCursor.reset(cursor);
}

void CBatting::SetBall(CBall* ball)
{
	m_pBall.reset(ball);
}

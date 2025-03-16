#include "Pitching.h"
#include "Input.h"
#include "Collision.h"
#include "SceneGame.h"
#include "BallCount.h"

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
	static float fPitchTime = 0.0f;
	static DirectX::XMFLOAT2 fCursorPos = { 0.0f,0.0f };
	CBallCount* pBallCount = CBallCount::GetInstance().get();

	switch (m_nPitchingPhase)
	{
	case (int)CPitching::PitchingPhase::Set:
		if (IsKeyTrigger(VK_SPACE))
		{
			fCursorPos = m_pCursor->GetPos((int)CSceneGame::Playing::Defence);
			m_nPitchingPhase = (int)PitchingPhase::Pitch;
			fPitchTime = 0.0f;
			m_fSpeed = 148.0f - (float)(rand() % 3);
		}
		break;
	case (int)CPitching::PitchingPhase::Pitch:
		fPitchTime += 1.0f / 60.0f;
		if (IsKeyTrigger(VK_SPACE))
		{
			if (fPitchTime < 2.0f) INFO_MESSAGE("ミス");
			else if (fPitchTime < 2.5f)INFO_MESSAGE("ナイスピッチ");
			else if (fPitchTime < 2.7f)INFO_MESSAGE("ベストピッチ");
			else if (fPitchTime < 4.2f)INFO_MESSAGE("ナイスピッチ");
			else INFO_MESSAGE("ミス");

			m_nPitchingPhase = (int)CPitching::PitchingPhase::Release;
			fPitchTime = 0.0f;
		}
		else if(fPitchTime >= 4.2f)
		{
			INFO_MESSAGE("ミス");
			m_nPitchingPhase = (int)CPitching::PitchingPhase::Release;
			fPitchTime = 0.0f;
		}
		m_fChatchTime = SPEED_AJUST / KMETER(m_fSpeed) * 60.0f * 60.0f;
		break;
	case (int)CPitching::PitchingPhase::Release:
		fPitchTime += 1.0f / 60.0f;
		if (fPitchTime >= m_fChatchTime)
		{
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

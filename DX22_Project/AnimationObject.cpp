#include "AnimationObject.h"

CAnimationObject::CAnimationObject()
	: CGameObject()
	, m_bIsAnimation(false), m_bIsPause(false)
	, m_fTime(0.0f), m_fDurationTime(1.0f)
{

}

CAnimationObject::~CAnimationObject()
{

}

void CAnimationObject::Update()
{
	if (m_bIsAnimation && !m_bIsPause)
	{
		if (m_fTime < m_fDurationTime) ExecAnimation();
		else m_bIsAnimation = false;
		m_fTime += 1.0f / fFPS;
	}
	CGameObject::Update();
}

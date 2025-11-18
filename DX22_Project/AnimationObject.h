#pragma once

#include "GameObject.h"

class CAnimationObject : public CGameObject
{
public:
	CAnimationObject();
	~CAnimationObject();
	void Update() override;
	virtual bool IsAnimation() { return m_bIsAnimation; };
	virtual bool IsCompliteAnimation() { return m_fTime >= m_fDurationTime; }
	void StartAnimation() { m_bIsAnimation = true; }
	void PauseAnimation() { m_bIsPause = true; }
	virtual void EndAnimation() { m_bIsAnimation = false; }
	virtual void ExecAnimation() = 0;

protected:
	bool m_bIsAnimation;
	bool m_bIsPause;
	float m_fTime;
	float m_fDurationTime;

};

#pragma once

#include "Effect.h"
#include "Camera.h"

class CEffectGoal : public Effect
{
public:
	CEffectGoal();
	virtual ~CEffectGoal() override;
	void Draw() override;

	void SetCamera(CCamera* pCamera);

private:
	CCamera* m_pCamera;
};
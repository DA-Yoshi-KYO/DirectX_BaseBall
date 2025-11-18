#pragma once

#include "AnimationObject.h"

class CTitleBat : public CAnimationObject
{
public:
	CTitleBat();
	~CTitleBat();
	void Init() override;
	void ExecAnimation() override;
	void EndAnimation() override;
	void SetIsRight(bool isRight) { m_bIsRight = isRight; }

private:
	bool m_bIsRight;
};

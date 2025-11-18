#pragma once

#include "AnimationObject.h"

class CTitleBall : public CAnimationObject
{
public:
	CTitleBall();
	~CTitleBall();
	void Init() override;
	void ExecAnimation() override;
	void EndAnimation() override;

};

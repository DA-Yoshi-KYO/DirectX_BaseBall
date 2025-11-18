#pragma once

#include "AnimationObject.h"

class CTitleLogo : public CAnimationObject
{
public:
	CTitleLogo();
	~CTitleLogo();
	void Init() override;
	void ExecAnimation() override;

};

#pragma once

#include "AnimationObject.h"

class CTitleEndButton : public CAnimationObject
{
public:
	CTitleEndButton();
	~CTitleEndButton();
	void Init() override;
	void ExecAnimation() override;
	void EndAnimation() override;

};

#pragma once
#include "EffectBase.h"

class CParticleBalloon : public CEffectBase
{
public:
	CParticleBalloon();
	~CParticleBalloon();
	void Init() override;
	void OnDestroy() override;

};


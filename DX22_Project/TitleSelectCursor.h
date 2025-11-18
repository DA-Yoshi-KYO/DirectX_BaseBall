#pragma once

#include "GameObject.h"

class CTitleSelectCursor : public CGameObject
{
public:
	CTitleSelectCursor();
	~CTitleSelectCursor();
	void Init() override;
	void Update() override;

};

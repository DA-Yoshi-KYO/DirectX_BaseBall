#pragma once

#include "GameObject.h"

class CMemberSelectCursor : public CGameObject
{
public:
	CMemberSelectCursor();
	~CMemberSelectCursor();
	void Init() override;
	void Update() override;

private:
	float m_fTime = 0.0f;

};

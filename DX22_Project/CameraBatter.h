#pragma once

#include "Camera.h"

class CCameraBatter : public CCamera
{
public:
	CCameraBatter();
	virtual ~CCameraBatter();
	virtual void Update() override;
private:
	float m_radXZ;
	float m_radY;
	float m_radius;
};

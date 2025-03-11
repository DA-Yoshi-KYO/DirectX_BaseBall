#pragma once

#include "Camera.h"
#include "Input.h"

#define CAMERA_SPEED (0.5f)
#define CAMERA_ROTATE (0.01f)

class CCameraDebug : public CCamera
{
public:
	CCameraDebug();
	~CCameraDebug();
	void Update() final;
private:
	float m_radXZ;
	float m_radY;
	float m_radius;
};
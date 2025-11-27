#pragma once

#include "Camera.h"

#define CAMERA_SPEED (0.001f)
#define CAMERA_ROTATE (0.01f)

class CCameraDebug : public CCamera
{
public:
	CCameraDebug();
	~CCameraDebug();
	void Update() final;
	void SetMove(bool move) { m_bMove = move; }

private:
	float m_radXZ;
	float m_radY;
	float m_radius;
	bool m_bMove;

};
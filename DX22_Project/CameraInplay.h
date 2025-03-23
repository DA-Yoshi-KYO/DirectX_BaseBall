#pragma once

#include "Camera.h"

class CCameraInplay : public CCamera
{
public:
	CCameraInplay();
	virtual ~CCameraInplay();
	virtual void Update() override;
private:

};

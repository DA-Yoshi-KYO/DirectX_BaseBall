#pragma once

#include "Camera.h"

class CCameraEvent : public CCamera 
{
public:
	CCameraEvent();
	~CCameraEvent();

	void Update() override;
	void SetEvent(DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 endPos, float time);
	bool IsEvent();
private:
	DirectX::XMFLOAT3 m_start;
	DirectX::XMFLOAT3 m_end;
	float m_time;
	float m_totalTime;
};
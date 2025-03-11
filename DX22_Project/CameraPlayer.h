#pragma once

#include "Camera.h"
#include "Player.h"

class CCameraPlayer : public CCamera
{
public:
	CCameraPlayer();
	virtual ~CCameraPlayer() override;
	virtual void Update() override;
	DirectX::XMFLOAT3 GetForward();
	void SetPlayer(CPlayer* player);
private:
	float m_fYaw;
	float m_fPitch;
	DirectX::XMFLOAT3 m_tForward;
	CPlayer* m_pPlayer;
};
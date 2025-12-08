#pragma once

#include "GameObject.h"

#define MAX_PARTICLE (100)

struct PartcleParame
{
	DirectX::XMFLOAT3 fPos;
	DirectX::XMFLOAT3 fSize;
	DirectX::XMFLOAT4 fColor;
	DirectX::XMFLOAT3 fVelocity;
	DirectX::XMFLOAT3 fCreatePos;
	float fLifeTime;
	float fIntervalTime;
	bool bUse;
};

class CEffectBase :  public CGameObject
{
protected:
	PartcleParame m_tParticleParam[MAX_PARTICLE] = {};
	float m_fTime = 0.0f;
	int m_nMaxParticle = MAX_PARTICLE;
	DirectX::XMFLOAT3 m_fColliderSquareSize = {0.0f, 0.0f, 0.0f};
public:
	void Init()override;
	void Update()override;
	void OnColliderHit(CColliderBase* other)override;


	float RandomCalculation(int min, int max, unsigned int DecimalPlaces = 1);

	void SetPos(DirectX::XMFLOAT3 pos);
	void SetVelocityX(float velocityX);
	void SetVelocityY(float velocityY);
	void SetVelocityZ(float velocityZ);

	void SetCalculationSize(float CalculationSize);
	
	void SetColor(DirectX::XMFLOAT4 color);

	void SetLifeTime(float life);
};


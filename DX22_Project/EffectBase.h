#pragma once

#include "GameObject.h"
#include "SpriteRenderer.h"

struct PartcleParam
{
	DirectX::XMFLOAT3 m_f3Pos;
	DirectX::XMFLOAT3 m_f3Size;
	DirectX::XMFLOAT4 m_f4Color;
	DirectX::XMFLOAT3 m_f3Velocity;
	DirectX::XMFLOAT3 m_f3CreatePos;
	float m_fTime;
	float m_fDurationTime;
	bool m_bUse;
};

class CEffectBase :  public CGameObject
{
public:
	CEffectBase();
	virtual ~CEffectBase();
	void Init()override;
	void Update()override;
	void Draw()override;
	
protected:
	std::vector<PartcleParam> m_tParticleParam;
	CSpriteRenderer* m_pRenderer;

};


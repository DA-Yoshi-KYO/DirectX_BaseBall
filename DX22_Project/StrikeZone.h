#pragma once
#include "Collision.h"
#include "GameObject.h"

constexpr DirectX::XMFLOAT3 ce_fStrikeZoneSize{ 120.0f,120.0f,0.0f };

class CStrikeZone : public CGameObject
{
public:
	CStrikeZone();
	~CStrikeZone();
	void Init() override;
	void Update() override;
	
public:
	Collision::Info2D GetCollision();
	
private:
	// “–‚½‚è”»’èî•ñ
	Collision::Info2D m_Collision;

};
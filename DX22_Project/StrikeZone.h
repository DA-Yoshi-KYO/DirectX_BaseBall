#pragma once
#include "Texture.h"
#include "Defines.h"
#include "Collision.h"
#include "GameObject.h"

constexpr DirectX::XMFLOAT3 ce_fStrikeZoneSize{ 100.0f,100.0f,0.0f };

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
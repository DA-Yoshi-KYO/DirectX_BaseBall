#pragma once

#include "Texture.h"
#include "Defines.h"
#include "Collision.h"

constexpr DirectX::XMFLOAT2 ce_fStrikeZoneSize{ 100.0f,100.0f };

class CStrikeZone
{
private:
	CStrikeZone();
public:
	~CStrikeZone();
	void Update();
	void Draw();
	DirectX::XMFLOAT2 GetPos();
	DirectX::XMFLOAT2 GetSize();
	Collision::Info2D GetCollision();

	static std::unique_ptr<CStrikeZone>& GetInstance();
private:
	std::unique_ptr<Texture> m_pTexture;
	//std::unique_ptr<Texture> m_pTest;
	SpriteParam m_tParam;
	//SpriteParam m_tTest;
	Collision::Info2D m_Collision;
};
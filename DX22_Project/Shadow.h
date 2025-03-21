#pragma once

#include "Defines.h"
#include "Collision.h"
#include "Texture.h"

class CShadow
{
public:
	CShadow();
	~CShadow();
	void Update();
	void Draw();
	
	void SetObjectPos(DirectX::XMFLOAT3 pos);
	void SetObjectCollision(Collision::Info collision);
	void SetGroundCollision(Collision::Info collision);

private:
	std::unique_ptr<Texture> m_pTexture;
	SpriteParam m_tParam;
	DirectX::XMFLOAT3 m_fObjectPos;
	DirectX::XMFLOAT3 m_fShadowPos;
	Collision::Info m_Collision[2];
};

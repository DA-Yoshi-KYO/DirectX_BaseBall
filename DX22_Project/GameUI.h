#pragma once

#include "Texture.h" 
#include "Sprite.h" 

class CGameUI
{
public:
	CGameUI();
	virtual ~CGameUI();
	virtual void Update() = 0;
	virtual void Draw() = 0;
private:

protected:
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 proj;
	DirectX::XMFLOAT2 m_pos;
	DirectX::XMFLOAT2 m_size;
};
#pragma once

#include "Defines.h"
#include "Texture.h"
#include "StrikeZone.h"

class CCursor
{
public:
	CCursor();
	~CCursor();
	void Update(int play);
	void Draw(int play);
	void SetStrikeZone(CStrikeZone* zone);
	DirectX::XMFLOAT2 GetPos(int play);
	DirectX::XMFLOAT2 GetSize(int play);
private:
	std::unique_ptr<Texture> m_pTexture[2];
	SpriteParam m_tParam[2];
	std::unique_ptr<CStrikeZone> m_pStrikeZone;
};
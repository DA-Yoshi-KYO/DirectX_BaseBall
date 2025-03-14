#pragma once

#include "Defines.h"
#include "Texture.h"
#include "StrikeZone.h"

class CCursor
{
public:
	CCursor();
	~CCursor();
	void Update();
	void Draw();
	void SetStrikeZone(CStrikeZone* zone);
private:
	std::unique_ptr<Texture> m_pTexture;
	SpriteParam m_tParam;
	std::unique_ptr<CStrikeZone> m_pStrikeZone;
};
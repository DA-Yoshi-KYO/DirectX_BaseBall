#pragma once

#include "Texture.h"
#include "Defines.h"

class CStrikeZone
{
public:
	CStrikeZone();
	~CStrikeZone();
	void Update();
	void Draw();
private:
	std::unique_ptr<Texture> m_pTexture;
	SpriteParam m_tParam;
};
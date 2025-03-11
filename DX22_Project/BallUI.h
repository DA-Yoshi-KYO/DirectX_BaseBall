#pragma once

#include "Texture.h"
#include "Defines.h"

#define MAX_BALL 5

class CBallUI
{
public:
	CBallUI();
	~CBallUI();
	void Update();
	void Draw();
	void SetBallVal(int ball);
private:
	Texture* m_pTexture;
	SpriteParam m_Sprite[MAX_BALL];
	int m_nBall;
};

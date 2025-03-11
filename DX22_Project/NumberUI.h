#pragma once

#include "Texture.h"

class CNumberUI
{
public:
	enum nMode
	{
		Right_AL,
		Left_AL,
		Center_AL,
	};


	CNumberUI();
	~CNumberUI();
	void Draw();
	void Update();
	void SetNumber(int);
	void SetLend(int InData);
	void SetNumberColor(int InColor);
	void SetArrangment(nMode InMode);
private:
	Texture* m_pNumber[10];
	Texture* m_pPoint;
	int nNumber;
	int nLong;
	int nColor;
	float fAlpha;
	nMode NowMode;
};


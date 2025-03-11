#include "NumberUI.h"
#include "Main.h"

CNumberUI::CNumberUI()
	:m_pNumber{}
	,nNumber(),nLong(),nColor(0)
	,fAlpha(), NowMode(Center_AL)
{
	for (int i = 0; i < 10; i++)
	{
		m_pNumber[i] = new Texture();
	}
	m_pPoint = new Texture();
	m_pNumber[0]->Create("Assets/Texture/Number/0.png");
	m_pNumber[1]->Create("Assets/Texture/Number/1.png");
	m_pNumber[2]->Create("Assets/Texture/Number/2.png");
	m_pNumber[3]->Create("Assets/Texture/Number/3.png");
	m_pNumber[4]->Create("Assets/Texture/Number/4.png");
	m_pNumber[5]->Create("Assets/Texture/Number/5.png");
	m_pNumber[6]->Create("Assets/Texture/Number/6.png");
	m_pNumber[7]->Create("Assets/Texture/Number/7.png");
	m_pNumber[8]->Create("Assets/Texture/Number/8.png");
	m_pNumber[9]->Create("Assets/Texture/Number/9.png");
	m_pPoint->Create("Assets/Texture/Number/Point.png");

}

CNumberUI::~CNumberUI()
{
	for (int i = 0; i < 10; i++)
	{
		SAFE_DELETE(m_pNumber[i]);
	}
	SAFE_DELETE(m_pPoint);
}

void CNumberUI::Draw()
{

}

void CNumberUI::Update()
{

}

void CNumberUI::SetNumber(int number)
{
	nNumber = number;
	nLong = 1;
	int x;
	x = number;

	while (1)
	{
		x = x / 10;
		if (x)
		{
			nLong++;
		}
		else
		{
			break;
		}
	}
}

void CNumberUI::SetLend(int InData)
{
	nLong = InData;
}

void CNumberUI::SetNumberColor(int InColor)
{
	nColor = InColor;
	if (nColor > 2)
	{
		MessageBox(NULL, "nColorÉGÉâÅ[", "NumberUI.cpp", MB_OK);
		nColor = 2;
	}
}

void CNumberUI::SetArrangment(nMode InMode)
{
	NowMode = InMode;
}

#include "BallUI.h"
#include "Sprite.h"
#include <DirectXMath.h>

CBallUI::CBallUI()
	: m_pTexture(nullptr)
	, m_nBall(MAX_BALL)
{
	m_pTexture = new Texture();
	if(FAILED(m_pTexture->Create(TEXPASS("Ball.png"))))MessageBox(NULL,"Ball.png","Error",MB_OK);
	for (int i = 0; i < MAX_BALL; i++)
	{
		m_Sprite[i].pos = { -580.0f + i * 100.0f ,300.0f };
		m_Sprite[i].size = { 100.0f,100.0f };
		m_Sprite[i].rotate = 0.0f;
		m_Sprite[i].color = { 1.0f,1.0f,1.0f,1.0f };
		m_Sprite[i].uvPos = { 0.0f,0.0f };
		m_Sprite[i].uvSize = { 1.0f,1.0f };
	}
}

CBallUI::~CBallUI()
{
	SAFE_DELETE(m_pTexture);
}

void CBallUI::Update()
{
	for (int i = MAX_BALL - 1; i > m_nBall; i--)
	{
		m_Sprite[i].color = { 0.5f,0.5f,0.5f,1.0f };
	}
}

void CBallUI::Draw()
{
	for (int i = 0; i < MAX_BALL; i++)
	{
		Sprite::SetOffset(m_Sprite[i].pos);
		Sprite::SetSize(m_Sprite[i].size);
		Sprite::SetColor(m_Sprite[i].color);
		Sprite::SetUVPos(m_Sprite[i].uvPos);
		Sprite::SetUVScale(m_Sprite[i].uvSize);
		Sprite::SetWorld(m_Sprite[i].world);
		Sprite::SetView(m_Sprite[i].view);
		Sprite::SetProjection(m_Sprite[i].proj);
		Sprite::SetTexture(m_pTexture);
		Sprite::Draw();
	}
}

void CBallUI::SetBallVal(int ball)
{
	m_nBall = ball;
}

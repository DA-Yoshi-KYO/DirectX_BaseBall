#include "StrikeZone.h"
#include "Camera.h"
#include "Sprite.h"
#include "Main.h"

CStrikeZone::CStrikeZone()
	: m_pTexture(nullptr)
{
	m_pTexture = std::make_unique<Texture>();
	if (FAILED(m_pTexture->Create(TEXPASS("StrikeZone.png")))) MessageBox(NULL, "StrikeZone.png", "Error", MB_OK);
	//m_pTest = std::make_unique<Texture>();

	m_tParam.pos = { 0.0f,-150.0f };
	m_tParam.size = ce_fStrikeZoneSize;
	m_tParam.rotate = 0.0f;
	m_tParam.color = { 1.0f,1.0f,1.0f,1.0f };
	m_tParam.uvPos = { 0.0f,0.0f };
	m_tParam.uvSize = { 1.0f,1.0f };
	m_tParam.world = CCamera::Get2DWolrdMatrix();
	m_tParam.view = CCamera::Get2DViewMatrix();
	m_tParam.proj = CCamera::Get2DProjectionMatrix();

	//m_tTest.pos = { 0.0f,0.0f };
	//m_tTest.size = ce_fStrikeZoneSize;
	//m_tTest.rotate = 0.0f;
	//m_tTest.color = { 1.0f,1.0f,1.0f,1.0f };
	//m_tTest.uvPos = { 0.0f,0.0f };
	//m_tTest.uvSize = { 1.0f,1.0f };
	//m_tTest.world = CCamera::Get2DWolrdMatrix();
	//m_tTest.view = CCamera::Get2DViewMatrix();
	//m_tTest.proj = CCamera::Get2DProjectionMatrix();
	//
	m_Collision.type = Collision::Type2D::eSquare;
	m_Collision.square.pos = m_tParam.pos;
	m_Collision.square.size = m_tParam.size;
}

CStrikeZone::~CStrikeZone()
{
}

void CStrikeZone::Update()
{
	m_Collision.square.pos = m_tParam.pos;
	m_Collision.square.size = m_tParam.size;
}

void CStrikeZone::Draw()
{
	SetRender2D();
	Sprite::SetParam(m_tParam);
	Sprite::SetTexture(m_pTexture.get());
	Sprite::Draw();

	//static int a = 0;
	//static int b = 60;
	//static int p = 0;
	//constexpr int maxp = 50;
	//
	//if (a > b)
	//{
	//	m_tTest.uvPos.x	= (float)(p % 5) / 5.0f;
	//	m_tTest.uvPos.y	= (float)(p / 5) / 5.0f;
	//	a = 0;
	//	p++;
	//}
	//a++;

	//Sprite::SetParam(m_tTest);
	//Sprite::SetTexture(m_pTest.get());
	//Sprite::Draw();
}

DirectX::XMFLOAT2 CStrikeZone::GetPos()
{
	return m_tParam.pos;
}

DirectX::XMFLOAT2 CStrikeZone::GetSize()
{
	return m_tParam.size;
}

Collision::Info2D CStrikeZone::GetCollision()
{
	return m_Collision;
}

std::unique_ptr<CStrikeZone>& CStrikeZone::GetInstance()
{
	static std::unique_ptr<CStrikeZone> instance(new CStrikeZone());

	return instance;
}

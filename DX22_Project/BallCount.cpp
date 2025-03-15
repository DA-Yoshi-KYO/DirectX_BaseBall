#include "BallCount.h"
#include "Camera.h"
#include "Sprite.h"
#include "ImGuiManager.h"

#define COUNT_SPLIT_X 5
#define COUNT_SPLIT_Y 5

std::unique_ptr<CBallCount> CBallCount::m_pInstance = nullptr;

CBallCount::CBallCount()
{

}

CBallCount::~CBallCount()
{

}

void CBallCount::Init()
{
	m_pBack = std::make_unique<Texture>();
	if (FAILED(m_pBack->Create(TEXPASS("BallCount.png"))))ERROR_MESSAGE("BallCount.png");
	m_pSheet = std::make_unique<Texture>();
	if (FAILED(m_pSheet->Create(TEXPASS("BallCountSheet.png"))))ERROR_MESSAGE("BallCountSheet.png");

	DirectX::XMFLOAT4X4 wvp[3];
	wvp[0] = CCamera::Get2DWolrdMatrix();
	wvp[1] = CCamera::Get2DViewMatrix();
	wvp[2] = CCamera::Get2DProjectionMatrix();

	m_tBallParam.pos = { 0.0f,0.0f };
	m_tBallParam.size = { 32.0f,32.0f };
	m_tBallParam.rotate = 0.0f;
	m_tBallParam.color = { 0.0f,0.0f,1.0f,1.0f };
	m_tBallParam.uvPos = { 1.0f / (float)COUNT_SPLIT_X,2.0f / (float)COUNT_SPLIT_Y };
	m_tBallParam.uvSize = { 1.0f / (float)COUNT_SPLIT_X,1.0f / (float)COUNT_SPLIT_Y };
	m_tBallParam.world = wvp[0];
	m_tBallParam.view = wvp[1];
	m_tBallParam.proj = wvp[2];

	m_tBackParam.pos = { 0.0f,0.0f };
	m_tBackParam.size = { 128.0f,96.0f };
	m_tBackParam.rotate = 0.0f;
	m_tBackParam.color = { 1.0f,1.0f,1.0f,1.0f };
	m_tBackParam.uvPos = { 0.0f,0.0f };
	m_tBackParam.uvSize = { 1.0f,1.0f };
	m_tBackParam.world = wvp[0];
	m_tBackParam.view = wvp[1];
	m_tBackParam.proj = wvp[2];

	m_tCount.m_nBallCount = 0;
	m_tCount.m_nStrikeCount = 0;
	m_tCount.m_nOutCount = 0;
	for (int i = 0; i < MAX_BASE_COUNT; i++)
	{
		m_tCount.m_bBaseState[i] = false;
	}
	m_tCount.m_nScore[(int)Team::TOP] = 0;
	m_tCount.m_nScore[(int)Team::BOTTOM] = 0;
	m_tCount.m_nInning = 1;
	m_tCount.m_bTop = true;
}

void CBallCount::Update()
{

}

void CBallCount::Draw()
{
	// スコアボードの描画
	Sprite::SetParam(m_tBackParam);
	Sprite::SetTexture(m_pBack.get());
	Sprite::Draw();

	SpriteParamDebug(&m_tBackParam.pos,&m_tBackParam.size,"back");


	// ボールカウントの描画
	for (int i = 0; i < MAX_BALL_COUNT - 1; i++)
	{
		m_tBallParam.pos = { 32.0f * i,0.0f };

		if (m_tCount.m_nBallCount >= i + 1)m_tBallParam.color = { 0.0f,0.0f,1.0f,1.0f };
		else m_tBallParam.color = { 0.0f,0.0f,1.0f,0.3f };
		Sprite::SetParam(m_tBallParam);
		Sprite::SetTexture(m_pSheet.get());
		Sprite::Draw();
	}

	// ストライクカウントの描画
	for (int i = 0; i < MAX_STRIKE_COUNT - 1; i++)
	{
		m_tBallParam.pos = { 32.0f * i,32.0f };
		if (m_tCount.m_nStrikeCount >= i + 1)m_tBallParam.color = { 0.0f,0.0f,1.0f,1.0f };
		else m_tBallParam.color = { 0.0f,0.0f,1.0f,0.3f };
		Sprite::SetParam(m_tBallParam);
		Sprite::SetTexture(m_pSheet.get());
		Sprite::Draw();
	}

	// アウトカウントの描画
	for (int i = 0; i < MAX_OUT_COUNT - 1; i++)
	{
		m_tBallParam.pos = { 32.0f * i,64.0f };
		if (m_tCount.m_nOutCount >= i + 1)m_tBallParam.color = { 0.0f,0.0f,1.0f,1.0f };
		else m_tBallParam.color = { 0.0f,0.0f,1.0f,0.3f };
		Sprite::SetParam(m_tBallParam);
		Sprite::SetTexture(m_pSheet.get());
		Sprite::Draw();
	}
}

void CBallCount::AddBallCount()
{
	m_tCount.m_nBallCount++;
}

void CBallCount::AddStrikeCount()
{
	m_tCount.m_nStrikeCount++;
}

void CBallCount::AddOutCount()
{
	m_tCount.m_nOutCount++;
}

void CBallCount::SetBaseState(int base, bool state)
{
	m_tCount.m_bBaseState[base] = state;
}

void CBallCount::ResetCount()
{
	m_tCount.m_nStrikeCount = 0;
	m_tCount.m_nBallCount = 0;
	m_tCount.m_nOutCount = 0;
}

void CBallCount::ChangeInning()
{
	m_tCount.m_nStrikeCount = 0;
	m_tCount.m_nBallCount = 0;
	m_tCount.m_nOutCount = 0;
	for (int i = 0; i < MAX_BASE_COUNT; i++)
	{
		m_tCount.m_bBaseState[i] = false;
	}

	
	if (m_tCount.m_bTop && m_tCount.m_nInning == MAX_INNING)
	{
		if (m_tCount.m_nScore[(int)Team::TOP] > m_tCount.m_nScore[(int)Team::BOTTOM])
		{
			m_tCount.m_bEnd = true;
			return;
		}
	}
	if (m_tCount.m_nInning >= MAX_INNING)
	{
		if (!m_tCount.m_bTop && m_tCount.m_nScore[(int)Team::TOP] != m_tCount.m_nScore[(int)Team::BOTTOM])
		{
			m_tCount.m_bEnd = true;
			return;
		}
		else
		{
			if (!m_tCount.m_bTop)m_tCount.m_nInning++;
			m_tCount.m_bTop ^= true;
		}
	}
	else
	{
		if (!m_tCount.m_bTop)m_tCount.m_nInning++;
		m_tCount.m_bTop ^= true;
	}
}

bool CBallCount::IsEnd()
{
	return m_tCount.m_bEnd;
}

std::unique_ptr<CBallCount>& CBallCount::GetInstance()
{ 
	static std::unique_ptr<CBallCount> instance(new CBallCount());
	return instance;
}

void CBallCount::DestroyInstance()
{
	m_pInstance.release();
}

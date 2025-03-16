#include "BallCount.h"
#include "Camera.h"
#include "Sprite.h"
#include "ImGuiManager.h"

#define COUNT_SPLIT_X 5
#define COUNT_SPLIT_Y 5

constexpr DirectX::XMFLOAT2 ce_BackPos = { 480.0f,260.0f };
constexpr DirectX::XMFLOAT2 ce_BackSize = { 280.0f,170.0f };
constexpr DirectX::XMFLOAT2 ce_CountPos = { 395.0f,271.0f };
constexpr DirectX::XMFLOAT2 ce_CountSize = { 35.0f,35.0f };
constexpr float ce_CountY = 37.3f;
constexpr DirectX::XMFLOAT2 ce_BasePos = { 515.0f,210.0f };
constexpr DirectX::XMFLOAT2 ce_BaseSize = { 50.0f,40.0f };
constexpr DirectX::XMFLOAT2 ce_BaseAjust = { 42.0f,40.0f };
constexpr DirectX::XMFLOAT2 ce_ScoreTopPos = { 500.0f,320.0f };
constexpr DirectX::XMFLOAT2 ce_ScoreBottomPos = { 560.0f,320.0f };
constexpr DirectX::XMFLOAT2 ce_ScoreSize = { 50.0f,50.0f };
constexpr DirectX::XMFLOAT2 ce_ScoreAjust = { 30.0f,60.0f };
constexpr DirectX::XMFLOAT2 ce_InningPos = { 395.0f,320.0f };
constexpr DirectX::XMFLOAT2 ce_InningSize = { 50.0f,50.0f };
constexpr DirectX::XMFLOAT2 ce_InningAjust = { 30.0f,60.0f };
constexpr DirectX::XMFLOAT2 ce_TopBottomPos = { 435.0f,310.0f };
constexpr DirectX::XMFLOAT2 ce_TopBottomSize = { 50.0f,50.0f };
constexpr DirectX::XMFLOAT2 ce_TopBottomAjust = { 30.0f,60.0f };

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

	m_tSheetParam.world = wvp[0];
	m_tSheetParam.view = wvp[1];
	m_tSheetParam.proj = wvp[2];

	m_tBackParam.pos = ce_BackPos;
	m_tBackParam.size = ce_BackSize;
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
	if (m_tCount.m_nStrikeCount >= MAX_STRIKE_COUNT)
	{
		m_tCount.m_nOutCount++;
		ResetCount();
	}
	if (m_tCount.m_nBallCount >= MAX_BALL_COUNT)
	{
		if (!m_tCount.m_bBaseState[0])
		{
			m_tCount.m_bBaseState[0] = true;
		}
		else if (!m_tCount.m_bBaseState[1])
		{
			m_tCount.m_bBaseState[1] = true;
		}
		else if (!m_tCount.m_bBaseState[2])
		{
			m_tCount.m_bBaseState[2] = true;
		}
		else
		{
			// 点加算
			if (m_tCount.m_bTop)AddScore((int)Team::TOP);
			else AddScore((int)Team::BOTTOM);
		}
		ResetCount();
	}
	if (m_tCount.m_nOutCount >= MAX_OUT_COUNT)
	{
		ChangeInning();
	}
}

void CBallCount::Draw()
{
	// スコアボードの描画
	Sprite::SetParam(m_tBackParam);
	Sprite::SetTexture(m_pBack.get());
	Sprite::Draw();

	DrawBallCount();
	DrawBaseCount();
	DrawScore();
	DrawInning();
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

void CBallCount::AddScore(int No)
{
	if(m_tCount.m_nScore[No] < MAX_SCORE)m_tCount.m_nScore[No]++;
}

void CBallCount::SetBaseState(int base, bool state)
{
	m_tCount.m_bBaseState[base] = state;
}

void CBallCount::ResetCount()
{
	m_tCount.m_nStrikeCount = 0;
	m_tCount.m_nBallCount = 0;
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

void CBallCount::DrawBallCount()
{
#ifdef _IMGUI
	static DirectX::XMFLOAT2 debugpos = { 0.0f,0.0f };
	SpriteParamDebug(&m_tBackParam.pos, &m_tBackParam.size, "back");
	SpriteParamDebug(&debugpos, &debugpos, "count", { 220.0f,20.0f });
#endif // _IMGUI
	// ボールカウント共通の処理
	m_tSheetParam.size = ce_CountSize;
	m_tSheetParam.uvPos = { 1.0f / (float)COUNT_SPLIT_X,2.0f / (float)COUNT_SPLIT_Y };
	m_tSheetParam.uvSize = { 1.0f / (float)COUNT_SPLIT_X,1.0f / (float)COUNT_SPLIT_Y };
	// ボールカウントの描画
	for (int i = 0; i < MAX_BALL_COUNT - 1; i++)
	{
		m_tSheetParam.pos = { m_tSheetParam.size.x * i + ce_CountPos.x,0.0f + ce_CountPos.y };

		if (m_tCount.m_nBallCount >= i + 1)m_tSheetParam.color = { 0.0f,1.0f,0.0f,1.0f };
		else m_tSheetParam.color = { 0.0f,1.0f,0.0f,0.3f };
		Sprite::SetParam(m_tSheetParam);
		Sprite::SetTexture(m_pSheet.get());
		Sprite::Draw();
	}

	// ストライクカウントの描画
	for (int i = 0; i < MAX_STRIKE_COUNT - 1; i++)
	{
		m_tSheetParam.pos = { m_tSheetParam.size.x * i + ce_CountPos.x,-ce_CountY + ce_CountPos.y };
		if (m_tCount.m_nStrikeCount >= i + 1)m_tSheetParam.color = { 1.0f,1.0f,0.0f,1.0f };
		else m_tSheetParam.color = { 1.0f,1.0f,0.0f,0.3f };
		Sprite::SetParam(m_tSheetParam);
		Sprite::SetTexture(m_pSheet.get());
		Sprite::Draw();
	}

	// アウトカウントの描画
	for (int i = 0; i < MAX_OUT_COUNT - 1; i++)
	{
		m_tSheetParam.pos = { m_tSheetParam.size.x * i + ce_CountPos.x,-ce_CountY * 2.0f + ce_CountPos.y };
		if (m_tCount.m_nOutCount >= i + 1)m_tSheetParam.color = { 1.0f,0.0f,0.0f,1.0f };
		else m_tSheetParam.color = { 1.0f,0.0f,0.0f,0.3f };
		Sprite::SetParam(m_tSheetParam);
		Sprite::SetTexture(m_pSheet.get());
		Sprite::Draw();
	}
}

void CBallCount::DrawBaseCount()
{
#ifdef _IMGUI
	static DirectX::XMFLOAT2 debugpos = { 0.0f,0.0f };
	static DirectX::XMFLOAT2 debugsize = { 30.0f,30.0f };
	SpriteParamDebug(&debugpos, &debugsize, "count", { 220.0f,20.0f });
#endif // _IMGUI

	// ベースカウント共通の処理
	m_tSheetParam.uvPos = { 0.0f / (float)COUNT_SPLIT_X,2.0f / (float)COUNT_SPLIT_Y };
	m_tSheetParam.uvSize = { 1.0f / (float)COUNT_SPLIT_X,1.0f / (float)COUNT_SPLIT_Y };

	for (int i = 0; i < MAX_BASE_COUNT - 1; i++)
	{
		m_tSheetParam.size = ce_BaseSize;
		if(m_tCount.m_bBaseState[i])m_tSheetParam.color = {1.0f,1.0f,0.0f,1.0f};
		else m_tSheetParam.color = { 1.0f,1.0f,1.0f,1.0f };

		switch (i)
		{
		case 0:
			m_tSheetParam.pos = { ce_BasePos.x + ce_BaseAjust.x * 2.0f,ce_BasePos.y };
			break;
		case 1:
			m_tSheetParam.pos = { ce_BasePos.x + ce_BaseAjust.x,ce_BasePos.y + ce_BaseAjust.y };
			break;
		case 2:
			m_tSheetParam.pos = ce_BasePos;
			break;
		default:
			break;
		}
		Sprite::SetParam(m_tSheetParam);
		Sprite::SetTexture(m_pSheet.get());
		Sprite::Draw();
	}
}

void CBallCount::DrawScore()
{
#ifdef _IMGUI
	static DirectX::XMFLOAT2 debugpos = { 0.0f,0.0f };
	static DirectX::XMFLOAT2 debugsize = { 30.0f,30.0f };
	SpriteParamDebug(&debugpos, &debugsize, "score", { 220.0f,20.0f });
#endif // _IMGUI

	int nNum = 0;

	m_tSheetParam.uvSize = { 1.0f / (float)COUNT_SPLIT_X,1.0f / (float)COUNT_SPLIT_Y };
	m_tSheetParam.color = { 1.0f,1.0f,1.0f,1.0f };	
	m_tSheetParam.size = ce_ScoreSize;

	// 先行
	for (int i = 0; i < 2; i++)
	{
		switch (i)
		{
		case 0:
			nNum = m_tCount.m_nScore[(int)Team::TOP] % 10;
			break;
		case 1:
			if (m_tCount.m_nScore[(int)Team::TOP] < 10) continue;
			nNum = m_tCount.m_nScore[(int)Team::TOP] / 10;
			break;
		default:
			break;
		}
		m_tSheetParam.pos = { ce_ScoreTopPos.x - ce_ScoreAjust.x * i, ce_ScoreTopPos.y };
		m_tSheetParam.uvPos = { (float)(nNum % COUNT_SPLIT_X) / (float)COUNT_SPLIT_X ,(float)(nNum / COUNT_SPLIT_X) / (float)COUNT_SPLIT_Y };
		Sprite::SetParam(m_tSheetParam);
		Sprite::SetTexture(m_pSheet.get());
		Sprite::Draw();
	}

	// 後攻
	for (int i = 0; i < 2; i++)
	{
		switch (i)
		{
		case 0:
			nNum = m_tCount.m_nScore[(int)Team::BOTTOM] % 10;
			break;
		case 1:
			if (m_tCount.m_nScore[(int)Team::BOTTOM] < 10) continue;
			nNum = m_tCount.m_nScore[(int)Team::BOTTOM] / 10;
			break;
		default:
			break;
		}
		if (m_tCount.m_nScore[(int)Team::BOTTOM] < 10)m_tSheetParam.pos = { ce_ScoreBottomPos.x + ce_ScoreAjust.x * i, ce_ScoreBottomPos.y };
		else m_tSheetParam.pos = { ce_ScoreBottomPos.x + ce_ScoreAjust.x * abs(i - 1), ce_ScoreBottomPos.y};
		m_tSheetParam.uvPos = { (float)(nNum % COUNT_SPLIT_X) / (float)COUNT_SPLIT_X ,(float)(nNum / COUNT_SPLIT_X) / (float)COUNT_SPLIT_Y };
		Sprite::SetParam(m_tSheetParam);
		Sprite::SetTexture(m_pSheet.get());
		Sprite::Draw();
	}
}

void CBallCount::DrawInning()
{
#ifdef _IMGUI
	static DirectX::XMFLOAT2 debugpos = { 0.0f,0.0f };
	static DirectX::XMFLOAT2 debugsize = { 30.0f,30.0f };
	SpriteParamDebug(&debugpos, &debugsize, "top", { 220.0f,20.0f });
#endif // _IMGUI

	ValueDebug(&m_tCount.m_nInning, "InningNum", { 420.0f,20.0f });
	BoolDebug(&m_tCount.m_bTop, "IsTop", { 220.0f,20.0f });

	int nNum = 0;

	m_tSheetParam.size = ce_InningSize;
	m_tSheetParam.color = { 1.0f,1.0f,1.0f,1.0f };
	m_tSheetParam.uvSize = { 1.0f / (float)COUNT_SPLIT_X,1.0f / (float)COUNT_SPLIT_Y };
	for (int i = 0; i < 2; i++)
	{
		switch (i)
		{
		case 0:
			nNum = m_tCount.m_nInning % 10;
			break;
		case 1:
			if (m_tCount.m_nInning < 10) continue;
			nNum = m_tCount.m_nInning / 10;
			break;
		default:
			break;
		}
		m_tSheetParam.pos = { ce_InningPos.x - ce_InningAjust.x * i, ce_InningPos.y };
		m_tSheetParam.uvPos = { (float)(nNum % COUNT_SPLIT_X) / (float)COUNT_SPLIT_X ,(float)(nNum / COUNT_SPLIT_X) / (float)COUNT_SPLIT_Y };
		Sprite::SetParam(m_tSheetParam);
		Sprite::SetTexture(m_pSheet.get());
		Sprite::Draw();
	}
	Sprite::SetParam(m_tSheetParam);
	Sprite::SetTexture(m_pSheet.get());
	Sprite::Draw();

	m_tSheetParam.pos = ce_TopBottomPos;
	m_tSheetParam.size = ce_TopBottomSize;
	if (m_tCount.m_bTop)m_tSheetParam.uvPos = { 2.0f / (float)COUNT_SPLIT_X ,2.0f / (float)COUNT_SPLIT_Y };
	else m_tSheetParam.uvPos = { 3.0f / (float)COUNT_SPLIT_X ,2.0f / (float)COUNT_SPLIT_Y };
	Sprite::SetParam(m_tSheetParam);
	Sprite::SetTexture(m_pSheet.get());
	Sprite::Draw();
}

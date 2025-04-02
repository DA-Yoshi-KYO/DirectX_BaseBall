// ==============================
//    インクルード部
// ==============================
#include "BallCount.h"
#include "Camera.h"
#include "Sprite.h"
#include "ImGuiManager.h"
#include "Main.h"

// ==============================
//    定数定義
// ==============================
// スコアボードのパラメータ
constexpr DirectX::XMFLOAT2 ce_fBackPos = { 480.0f,260.0f };
constexpr DirectX::XMFLOAT2 ce_fBackSize = { 280.0f,170.0f };
// 各種カウントのパラメータ
constexpr DirectX::XMFLOAT2 ce_fCountPos = { 395.0f,271.0f };
constexpr DirectX::XMFLOAT2 ce_fCountSize = { 35.0f,35.0f };
constexpr float ce_fCountAjustY = 37.3f;
// ベース状況のパラメータ
constexpr DirectX::XMFLOAT2 ce_fBasePos = { 515.0f,210.0f };
constexpr DirectX::XMFLOAT2 ce_fBaseSize = { 50.0f,40.0f };
constexpr DirectX::XMFLOAT2 ce_fBaseAjust = { 42.0f,40.0f };
// 得点のパラメータ
constexpr DirectX::XMFLOAT2 ce_fScoreTopPos = { 500.0f,320.0f };
constexpr DirectX::XMFLOAT2 ce_fScoreBottomPos = { 560.0f,320.0f };
constexpr DirectX::XMFLOAT2 ce_fScoreSize = { 50.0f,50.0f };
constexpr DirectX::XMFLOAT2 ce_fScoreAjust = { 30.0f,60.0f };
// イニングのパラメータ
constexpr DirectX::XMFLOAT2 ce_fInningPos = { 395.0f,320.0f };
constexpr DirectX::XMFLOAT2 ce_fInningSize = { 50.0f,50.0f };
constexpr DirectX::XMFLOAT2 ce_fInningAjust = { 30.0f,60.0f };
// オモテ・ウラのパラメータ
constexpr DirectX::XMFLOAT2 ce_fTopBottomPos = { 435.0f,310.0f };
constexpr DirectX::XMFLOAT2 ce_fTopBottomSize = { 50.0f,50.0f };
constexpr DirectX::XMFLOAT2 ce_fTopBottomAjust = { 30.0f,60.0f };

CBallCount::CBallCount()
	: m_tCount{}, m_bInplay{}, m_bPlayer1Top(true), m_tGameState{}
{

}

CBallCount::~CBallCount()
{

}

void CBallCount::Init(InningHalf Player1Harf)
{
	// モデル読み込み
	m_pBack = std::make_unique<Texture>();
	if (FAILED(m_pBack->Create(TEXPASS("BallCount.png"))))ERROR_MESSAGE("BallCount.png");
	m_pSheet = std::make_unique<Texture>();
	if (FAILED(m_pSheet->Create(TEXPASS("BallCountSheet.png"))))ERROR_MESSAGE("BallCountSheet.png");

	// [0]:world
	// [1]:view
	// [2]:projection
	DirectX::XMFLOAT4X4 wvp[3];
	wvp[0] = CCamera::Get2DWolrdMatrix();
	wvp[1] = CCamera::Get2DViewMatrix();
	wvp[2] = CCamera::Get2DProjectionMatrix();

	// モデルパラメータの設定
	m_tSheetParam.world = wvp[0];
	m_tSheetParam.view = wvp[1];
	m_tSheetParam.proj = wvp[2];

	m_tBackParam.pos = ce_fBackPos;
	m_tBackParam.size = ce_fBackSize;
	m_tBackParam.rotate = 0.0f;
	m_tBackParam.color = { 1.0f,1.0f,1.0f,1.0f };
	m_tBackParam.uvPos = { 0.0f,0.0f };
	m_tBackParam.uvSize = { 1.0f,1.0f };
	m_tBackParam.world = wvp[0];
	m_tBackParam.view = wvp[1];
	m_tBackParam.proj = wvp[2];

	// 各要素の初期化
	m_tCount.m_nBallCount = 0;
	m_tCount.m_nStrikeCount = 0;
	m_tCount.m_nOutCount = 0;
	for (int i = 0; i < MAX_BASE_COUNT; i++)
	{
		m_tCount.m_bBaseState[i] = false;
	}
	m_tCount.m_nScore[(int)Team::Player1] = 0;
	m_tCount.m_nScore[(int)Team::Player2] = 0;
	m_tCount.m_nInning = 1;
	m_tGameState.half = InningHalf::Top;
	switch (Player1Harf)
	{
	case CBallCount::InningHalf::Top:
		m_tGameState.offense = Team::Player1;
		m_tGameState.defense = Team::Player2;
		m_bPlayer1Top = true;
		break;
	case CBallCount::InningHalf::Bottom:
		m_tGameState.defense = Team::Player1;
		m_tGameState.offense = Team::Player2;
		m_bPlayer1Top = false;
		break;
	default:
		break;
	}

	for (int i = 0; i < (int)InplayElement::Max; i++)
	{
		m_bInplay[i] = false;
	}
}

void CBallCount::Update()
{
	// ストライクが3つ溜まったら
	if (m_tCount.m_nStrikeCount >= MAX_STRIKE_COUNT)
	{
		//アウトカウントを増やし、ストライク・ボールのカウントをリセットする
		m_tCount.m_nOutCount++;
		ResetCount();
	}

	// ボールが4つ溜まったら
	if (m_tCount.m_nBallCount >= MAX_BALL_COUNT)
	{
		// 空いてる塁を一塁から探し
		// 空いていたら塁を埋める
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
		// どこも空いていなかったら満塁なので点を入れる
		else
		{
			switch (m_tGameState.offense)
			{
			case CBallCount::Team::Player1:
				break;
			case CBallCount::Team::Player2:
				break;
			default:
				break;
			}
		}
		// ストライク・ボールのカウントをリセットする
		ResetCount();
	}

	// アウトが3つ溜まったら
	if (m_tCount.m_nOutCount >= MAX_OUT_COUNT)
	{
		// イニング(オモテ・ウラ)を終える
		ChangeInning();
	}
}

void CBallCount::Draw()
{
	SetRender2D();
	// スコアボードの描画
	Sprite::SetParam(m_tBackParam);
	Sprite::SetTexture(m_pBack.get());
	Sprite::Draw();

	// ボールカウントの描画
	DrawBallCount();

	// 塁状況の描画
	DrawBaseCount();

	// 得点の描画
	DrawScore();

	// イニングの描画
	DrawInning();
}

void CBallCount::AddBallCount()
{
	m_tCount.m_nBallCount++;
}

void CBallCount::AddStrikeCount(bool isFoul)
{
	// ファールの時は2ストライク未満の時だけストライクカウントを加算する
	if (isFoul)
	{
		if (m_tCount.m_nStrikeCount < 2) m_tCount.m_nStrikeCount++;
	}
	else m_tCount.m_nStrikeCount++;
}

void CBallCount::AddOutCount()
{
	m_tCount.m_nOutCount++;
}

void CBallCount::AddScore()
{
	if ((int)m_tGameState.offense >= 2)
	{
		RANGEERROR_MESSAGE("m_tGameState.offense");
		return;
	}
	if (m_tCount.m_nScore[(int)m_tGameState.offense] < MAX_SCORE)m_tCount.m_nScore[(int)m_tGameState.offense]++;
}

void CBallCount::SetBaseState(int base, bool state)
{
	m_tCount.m_bBaseState[base] = state;
}

bool CBallCount::GetBaseState(int base)
{
	return m_tCount.m_bBaseState[base];
}

void CBallCount::ResetCount()
{
	m_tCount.m_nStrikeCount = 0;
	m_tCount.m_nBallCount = 0;
}

void CBallCount::ChangeInning()
{
	// 各種カウントのリセット
	m_tCount.m_nStrikeCount = 0;
	m_tCount.m_nBallCount = 0;
	m_tCount.m_nOutCount = 0;

	// 塁状況のリセット
	for (int i = 0; i < MAX_BASE_COUNT; i++)
	{
		m_tCount.m_bBaseState[i] = false;
	}

	// 9回の表終了時で後攻が勝っていたら試合を終了する
	if (m_tGameState.half == InningHalf::Top && m_tCount.m_nInning == MAX_INNING)
	{
		if (m_bPlayer1Top)
		{
			if (m_tCount.m_nScore[(int)Team::Player1] < m_tCount.m_nScore[(int)Team::Player2])
			{
				m_tCount.m_bEnd = true;
				return;
			}
		}
		else
		{
			if (m_tCount.m_nScore[(int)Team::Player2] < m_tCount.m_nScore[(int)Team::Player1])
			{
				m_tCount.m_bEnd = true;
				return;
			}
		}
	}

	// 12回終了時の時の処理
	if (m_tCount.m_nInning >= MAX_INNING && m_tGameState.half == InningHalf::Bottom)
	{
		m_tCount.m_bEnd = true;
	}
	// 9回以上の時の処理
	else if (m_tCount.m_nInning >= MAX_INNING - 3)
	{
		// 後攻終了時に点数が違ったら試合を終了する
		if (m_tGameState.half != InningHalf::Bottom && m_tCount.m_nScore[(int)Team::Player1] != m_tCount.m_nScore[(int)Team::Player1])
		{
			m_tCount.m_bEnd = true;
			return;
		}
		// 同じなら延長線に突入する
		else
		{
			std::swap(m_tGameState.offense, m_tGameState.defense);
			switch (m_tGameState.half)
			{
			case InningHalf::Top:
				m_tGameState.half = InningHalf::Bottom;
				break;
			case InningHalf::Bottom:
				m_tGameState.half = InningHalf::Top;
				m_tCount.m_nInning++;
				break;
			default:
				break;
			}
		}
	}
	// 8回までは通常通り処理をする
	else
	{
		// 裏の時はイニングを進める
		std::swap(m_tGameState.offense, m_tGameState.defense);
		switch (m_tGameState.half)
		{
		case InningHalf::Top:
			m_tGameState.half = InningHalf::Bottom;
			break;
		case InningHalf::Bottom:
			m_tGameState.half = InningHalf::Top;
			m_tCount.m_nInning++;
			break;
		default:
			break;
		}
	}
}

bool CBallCount::IsEnd()
{
	return m_tCount.m_bEnd;
}

CBallCount::Team CBallCount::GetOffenseTeam()
{
	return m_tGameState.offense;
}

CBallCount::Team CBallCount::GetDefenseTeam()
{
	return m_tGameState.defense;
}

void CBallCount::SetEndInplay(InplayElement ElemEndInplay, bool state)
{
	m_bInplay[(int)ElemEndInplay] = state;
}

bool CBallCount::GetEndInplay()
{
	static float fTime = 0.0f;
	for (int i = 0; i < (int)InplayElement::Max; i++)
	{
		if(!m_bInplay[i])
		{
			fTime = 0.0f;
			return false;
		}
	}

	fTime += 1.0f / fFPS;

	if (fTime >= 3.0f)
	{
		return true;
	}

	return false;
}

std::unique_ptr<CBallCount>& CBallCount::GetInstance()
{ 
	// インスタンスは一つしか存在しない
	static std::unique_ptr<CBallCount> instance(new CBallCount());
	return instance;
}

void CBallCount::DrawBallCount()
{
#ifdef _IMGUI
	static DirectX::XMFLOAT2 debugpos = { 0.0f,0.0f };
	SpriteParamDebug(&m_tBackParam.pos, &m_tBackParam.size, "back");
	SpriteParamDebug(&debugpos, &debugpos, "count", { 220.0f,20.0f });
#endif // _IMGUI

	// ボールカウント共通の処理
	m_tSheetParam.size = ce_fCountSize;
	m_tSheetParam.uvPos = { 1.0f / (float)ce_nCountSplitX,2.0f / (float)ce_nCountSplitY };
	m_tSheetParam.uvSize = { 1.0f / (float)ce_nCountSplitX,1.0f / (float)ce_nCountSplitY };

	// ボールカウントの描画
	for (int i = 0; i < MAX_BALL_COUNT - 1; i++)
	{
		m_tSheetParam.pos = { m_tSheetParam.size.x * i + ce_fCountPos.x,0.0f + ce_fCountPos.y };

		if (m_tCount.m_nBallCount >= i + 1)m_tSheetParam.color = { 0.0f,1.0f,0.0f,1.0f };
		else m_tSheetParam.color = { 0.0f,1.0f,0.0f,0.3f };
		Sprite::SetParam(m_tSheetParam);
		Sprite::SetTexture(m_pSheet.get());
		Sprite::Draw();
	}

	// ストライクカウントの描画
	for (int i = 0; i < MAX_STRIKE_COUNT - 1; i++)
	{
		m_tSheetParam.pos = { m_tSheetParam.size.x * i + ce_fCountPos.x,-ce_fCountAjustY + ce_fCountPos.y };
		if (m_tCount.m_nStrikeCount >= i + 1)m_tSheetParam.color = { 1.0f,1.0f,0.0f,1.0f };
		else m_tSheetParam.color = { 1.0f,1.0f,0.0f,0.3f };
		Sprite::SetParam(m_tSheetParam);
		Sprite::SetTexture(m_pSheet.get());
		Sprite::Draw();
	}

	// アウトカウントの描画
	for (int i = 0; i < MAX_OUT_COUNT - 1; i++)
	{
		m_tSheetParam.pos = { m_tSheetParam.size.x * i + ce_fCountPos.x,-ce_fCountAjustY * 2.0f + ce_fCountPos.y };
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

	// 塁状況共通の処理
	m_tSheetParam.uvPos = { 0.0f / (float)ce_nCountSplitX,2.0f / (float)ce_nCountSplitY };
	m_tSheetParam.uvSize = { 1.0f / (float)ce_nCountSplitX,1.0f / (float)ce_nCountSplitY };

	// 塁状況の描画
	for (int i = 0; i < MAX_BASE_COUNT - 1; i++)
	{
		m_tSheetParam.size = ce_fBaseSize;
		if(m_tCount.m_bBaseState[i])m_tSheetParam.color = {1.0f,1.0f,0.0f,1.0f};
		else m_tSheetParam.color = { 1.0f,1.0f,1.0f,1.0f };

		switch (i)
		{
		case 0:
			m_tSheetParam.pos = { ce_fBasePos.x + ce_fBaseAjust.x * 2.0f,ce_fBasePos.y };
			break;
		case 1:
			m_tSheetParam.pos = { ce_fBasePos.x + ce_fBaseAjust.x,ce_fBasePos.y + ce_fBaseAjust.y };
			break;
		case 2:
			m_tSheetParam.pos = ce_fBasePos;
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

	// スプライトシート操作用
	int nNum = 0;

	// 得点共通の処理
	m_tSheetParam.uvSize = { 1.0f / (float)ce_nCountSplitX,1.0f / (float)ce_nCountSplitX };
	m_tSheetParam.color = { 1.0f,1.0f,1.0f,1.0f };	
	m_tSheetParam.size = ce_fScoreSize;

	// Player2の得点の描画
	for (int i = 0; i < 2; i++)
	{
		switch (i)
		{
		case 0:
			nNum = m_tCount.m_nScore[(int)Team::Player2] % 10;
			break;
		case 1:
			if (m_tCount.m_nScore[(int)Team::Player2] < 10) continue;
			nNum = m_tCount.m_nScore[(int)Team::Player2] / 10;
			break;
		default:
			break;
		}
		m_tSheetParam.pos = { ce_fScoreTopPos.x - ce_fScoreAjust.x * i, ce_fScoreTopPos.y };
		m_tSheetParam.uvPos = { (float)(nNum % ce_nCountSplitX) / (float)ce_nCountSplitX ,(float)(nNum / ce_nCountSplitX) / (float)ce_nCountSplitY };
		Sprite::SetParam(m_tSheetParam);
		Sprite::SetTexture(m_pSheet.get());
		Sprite::Draw();
	}

	// Player1の得点の描画
	for (int i = 0; i < 2; i++)
	{
		switch (i)
		{
		case 0:
			nNum = m_tCount.m_nScore[(int)Team::Player1] % 10;
			break;
		case 1:
			if (m_tCount.m_nScore[(int)Team::Player1] < 10) continue;
			nNum = m_tCount.m_nScore[(int)Team::Player1] / 10;
			break;
		default:
			break;
		}
		if (m_tCount.m_nScore[(int)Team::Player1] < 10)m_tSheetParam.pos = { ce_fScoreBottomPos.x + ce_fScoreAjust.x * i, ce_fScoreBottomPos.y };
		else m_tSheetParam.pos = { ce_fScoreBottomPos.x + ce_fScoreAjust.x * abs(i - 1), ce_fScoreBottomPos.y};
		m_tSheetParam.uvPos = { (float)(nNum % ce_nCountSplitX) / (float)ce_nCountSplitX ,(float)(nNum / ce_nCountSplitX) / (float)ce_nCountSplitY };
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
	ValueDebug(&m_tCount.m_nInning, "InningNum", { 420.0f,20.0f });
	BoolDebug(&m_tCount.m_bTop, "IsTop", { 220.0f,20.0f });
#endif // _IMGUI

	// スプライトシート操作用
	int nNum = 0;

	// イニング共通の処理
	m_tSheetParam.size = ce_fInningSize;
	m_tSheetParam.color = { 1.0f,1.0f,1.0f,1.0f };
	m_tSheetParam.uvSize = { 1.0f / (float)ce_nCountSplitX,1.0f / (float)ce_nCountSplitY };

	// イニングの描画
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
		m_tSheetParam.pos = { ce_fInningPos.x - ce_fInningAjust.x * i, ce_fInningPos.y };
		m_tSheetParam.uvPos = { (float)(nNum % ce_nCountSplitX) / (float)ce_nCountSplitX ,(float)(nNum / ce_nCountSplitX) / (float)ce_nCountSplitY };
		Sprite::SetParam(m_tSheetParam);
		Sprite::SetTexture(m_pSheet.get());
		Sprite::Draw();
	}
	Sprite::SetParam(m_tSheetParam);
	Sprite::SetTexture(m_pSheet.get());
	Sprite::Draw();

	// オモテ・ウラの描画
	m_tSheetParam.pos = ce_fTopBottomPos;
	m_tSheetParam.size = ce_fTopBottomSize;
	switch (m_tGameState.half)
	{
	case InningHalf::Top:
		m_tSheetParam.uvPos = { 2.0f / (float)ce_nCountSplitX ,2.0f / (float)ce_nCountSplitY };
		break;
	case InningHalf::Bottom:
		m_tSheetParam.uvPos = { 3.0f / (float)ce_nCountSplitX ,2.0f / (float)ce_nCountSplitY };
		break;
	default:
		break;
	}
	Sprite::SetParam(m_tSheetParam);
	Sprite::SetTexture(m_pSheet.get());
	Sprite::Draw();
}

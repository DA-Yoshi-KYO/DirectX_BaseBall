#include "SceneTitle.h"
#include "Input.h"
#include "Main.h"
#include "Sprite.h"
#include "Camera.h"
#include "Easing.h"

constexpr DirectX::XMFLOAT4 ce_fStartEndBallPos = { -700.0f,700.0f, 0.0f,150.0f};
constexpr DirectX::XMFLOAT4 ce_fStartEndBatPos = { 0.0f,-800.0f, 0.0f, 20.0f};
constexpr DirectX::XMFLOAT4 ce_fStartEndLogoSize = { 0.0f, 0.0f, 1000.0f,600.0f };
constexpr DirectX::XMFLOAT4 ce_fStartEndStartButtonPos = { 1000.0f,-180.0f, 0.0f, -180.0f };
constexpr DirectX::XMFLOAT4 ce_fStartEndEndButtonPos = { 1000.0f,-300.0f, 0.0f, -300.0f };

CSceneTitle::CSceneTitle()
	: m_pTexture{}, m_eTitlePhase(TitlePhase::Animation)
	, m_bSelected(false), m_bCursorAnim(false), m_bAnime{false}, m_nSelectKind((int)SelectKind::Start)
{
	for (int i = 0; i < (int)TextureKind::Max; i++)
	{
		m_pTexture[i] = std::make_unique<Texture>();
		m_tParam[i].pos = { 0.0f,0.0f };
		m_tParam[i].offsetPos = { 0.0f,0.0f };
		m_tParam[i].size = { 400.0f,400.0f };
		m_tParam[i].rotate = 0.0f;
		m_tParam[i].color = { 1.0f,1.0f,1.0f,1.0f };
		m_tParam[i].uvPos = { 0.0f,0.0f };
		m_tParam[i].uvSize = { 1.0f,1.0f };
		m_tParam[i].world = CCamera::Get2DWolrdMatrix(m_tParam[i].pos, m_tParam[i].rotate);
		m_tParam[i].view = CCamera::Get2DViewMatrix();
		m_tParam[i].proj = CCamera::Get2DProjectionMatrix();
	}
	if (FAILED(m_pTexture[(int)TextureKind::Back]->Create(PATH_TEX("TitleBack.jpg")))) ERROR_MESSAGE("TitleBack.jpg");
	m_tParam[(int)TextureKind::Back].size = { SCREEN_WIDTH,SCREEN_HEIGHT };

	if (FAILED(m_pTexture[(int)TextureKind::Ball]->Create(PATH_TEX("Ball.png")))) ERROR_MESSAGE("Ball.png");
	m_tParam[(int)TextureKind::Ball].pos = { ce_fStartEndBallPos.x,ce_fStartEndBallPos.y };
	m_tParam[(int)TextureKind::Ball].size = { 300.0f,300.0f };

	if (FAILED(m_pTexture[(int)TextureKind::Bat]->Create(PATH_TEX("Bat.png")))) ERROR_MESSAGE("Bat.png");
	m_tParam[(int)TextureKind::Bat].pos = { ce_fStartEndBatPos.x,ce_fStartEndBatPos.y };
	m_tParam[(int)TextureKind::Bat].size = { 600.0f,600.0f };

	m_pTexture[(int)TextureKind::Bat2].reset(m_pTexture[(int)TextureKind::Bat].get());
	m_tParam[(int)TextureKind::Bat2].pos = { ce_fStartEndBatPos.x,ce_fStartEndBatPos.y };
	m_tParam[(int)TextureKind::Bat2].size = { 600.0f,600.0f };

	if (FAILED(m_pTexture[(int)TextureKind::Logo]->Create(PATH_TEX("TitleLogo.png")))) ERROR_MESSAGE("TitleLogo.png");
	m_tParam[(int)TextureKind::Logo].pos = { 0.0f,-20.0f };
	m_tParam[(int)TextureKind::Logo].size = { ce_fStartEndLogoSize.x,ce_fStartEndLogoSize.y };

	if (FAILED(m_pTexture[(int)TextureKind::Start]->Create(PATH_TEX("Fade.png")))) ERROR_MESSAGE("Fade.png");
	m_tParam[(int)TextureKind::Start].pos = { ce_fStartEndStartButtonPos.x,ce_fStartEndStartButtonPos.y };
	m_tParam[(int)TextureKind::Start].size = { 300.0f,100.0 };

	if (FAILED(m_pTexture[(int)TextureKind::End]->Create(PATH_TEX("Fade.png")))) ERROR_MESSAGE("Fade.png");
	m_tParam[(int)TextureKind::End].pos = { ce_fStartEndEndButtonPos.x,ce_fStartEndEndButtonPos.y };
	m_tParam[(int)TextureKind::End].size = { 300.0f,100.0 };

	if (FAILED(m_pTexture[(int)TextureKind::Cursor]->Create(PATH_TEX("PositionSeeat.png")))) ERROR_MESSAGE("PositionSeeat.png");
	m_tParam[(int)TextureKind::Cursor].pos = { -200.0f, ce_fStartEndStartButtonPos.y };
	m_tParam[(int)TextureKind::Cursor].size = { 100.0f,100.0 };
	m_tParam[(int)TextureKind::Cursor].color = { 0.0f,0.0f,0.0f,1.0f };
	m_tParam[(int)TextureKind::Cursor].uvPos = { 3.0f / (float)ce_nSheetSplit, 2.0f / (float)ce_nSheetSplit };
	m_tParam[(int)TextureKind::Cursor].uvSize = { 1.0f / (float)ce_nSheetSplit, 1.0f / (float)ce_nSheetSplit };
}

CSceneTitle::~CSceneTitle()
{
	m_pTexture[(int)TextureKind::Bat2].release();
}

void CSceneTitle::Update()
{

	switch (m_eTitlePhase)
	{
	case CSceneTitle::TitlePhase::Animation: UpdateAnimation();  break;
	case CSceneTitle::TitlePhase::Select: UpdateSelect(); break;
	default: break;
	}
}

void CSceneTitle::Draw()
{
	SetRender2D();

	for (int i = 0; i < (int)TextureKind::Max; i++)
	{
		if (i == (int)TextureKind::Cursor && !m_bAnime[(int)AnimePhase::Buttons])continue;
		m_tParam[i].world = CCamera::Get2DWolrdMatrix(m_tParam[i].pos, m_tParam[i].rotate);
		Sprite::SetParam(m_tParam[i]);
		Sprite::SetTexture(m_pTexture[i].get());
		Sprite::Draw();
	}
}

void CSceneTitle::UpdateAnimation()
{
	if (IsKeyTrigger(VK_RETURN))
	{
		m_eTitlePhase = TitlePhase::Select;
	}

	// 現在のアニメーションが終わるまで次のアニメーションを行わない疑似コルーチン
	if (BallAnimation())
	{
		if (BatAnimation())
		{
			if (LogoAnimation())
			{

				m_eTitlePhase = TitlePhase::Select;
			}
		}
	}
}

void CSceneTitle::UpdateSelect()
{
	ResetSpriteParam();
	

	if (ButtonAnimation())
	{
		static float fTime = 0.0f;
		constexpr float ce_fSwapTime = 1.0f;
		if (!m_bCursorAnim)
		{
			m_tParam[(int)TextureKind::Cursor].uvPos.y = (1.0f / (float)ce_nSheetSplit) * (fTime * ce_fSwapTime) + 2.0f / (float)ce_nSheetSplit;
			m_tParam[(int)TextureKind::Cursor].uvSize.y = (-1.0f / (float)ce_nSheetSplit * 1.0f) * (fTime * ce_fSwapTime) + 3.0f / (float)ce_nSheetSplit;
			fTime += 1.0f / fFPS;

			if (m_tParam[(int)TextureKind::Cursor].uvPos.y >= 3.0f / (float)ce_nSheetSplit)
			{
				fTime = 0.0f;
				m_bCursorAnim = true;
			}
		}
		else
		{
			m_tParam[(int)TextureKind::Cursor].uvPos.y = (-1.0f / (float)ce_nSheetSplit) * (fTime * ce_fSwapTime) + 3.0f / (float)ce_nSheetSplit;
			m_tParam[(int)TextureKind::Cursor].uvSize.y = (1.0f / (float)ce_nSheetSplit * 1.0f) * (fTime * ce_fSwapTime) + 2.0f / (float)ce_nSheetSplit;
			fTime += 1.0f / fFPS;

			if (m_tParam[(int)TextureKind::Cursor].uvPos.y <= 2.0f / (float)ce_nSheetSplit)
			{
				fTime = 0.0f;
				m_bCursorAnim = false;
			}
		}

		if (IsKeyTrigger(VK_UP))
		{
			m_nSelectKind--;
			if (m_nSelectKind < (int)SelectKind::Start) m_nSelectKind = (int)SelectKind::End;
		}

		if (IsKeyTrigger(VK_DOWN))
		{
			m_nSelectKind++;
			if (m_nSelectKind > (int)SelectKind::End) m_nSelectKind = (int)SelectKind::Start;
		}

		if (IsKeyTrigger(VK_RETURN) && !m_bSelected)
		{
			switch (m_nSelectKind)
			{
			case (int)CSceneTitle::SelectKind::Start:
				m_tParam[(int)TextureKind::Cursor].pos.y = m_tParam[(int)TextureKind::Start].pos.y;
				SetNext(SceneKind::TeamSelect);
				break;
			case (int)CSceneTitle::SelectKind::End:
				m_tParam[(int)TextureKind::Cursor].pos.y = m_tParam[(int)TextureKind::End].pos.y;
				SetEnd(true);
				break;
			default:
				break;
			}
			m_bSelected = true;
		}
	}

	switch (m_nSelectKind)
	{
	case (int)CSceneTitle::SelectKind::Start:
		m_tParam[(int)TextureKind::Cursor].pos.y = m_tParam[(int)TextureKind::Start].pos.y;
		break;
	case (int)CSceneTitle::SelectKind::End:
		m_tParam[(int)TextureKind::Cursor].pos.y = m_tParam[(int)TextureKind::End].pos.y;
		break;
	default:
		break;
	}
}

bool CSceneTitle::BallAnimation()
{
	constexpr float easePosXTime = 3.5f;
	constexpr float easeRotateTime = 3.3f;
	constexpr float easePosYTime = 2.0f;
	static float fTime = 0.0f;

	if (m_bAnime[(int)AnimePhase::Ball])
	{
		return true;
	}

	
	m_tParam[(int)TextureKind::Ball].pos.x = easeOutBack(fTime, easePosXTime, ce_fStartEndBallPos.x, ce_fStartEndBallPos.z, 3.0f);
	m_tParam[(int)TextureKind::Ball].pos.y = easeOutBounce(fTime, easePosYTime, ce_fStartEndBallPos.y, ce_fStartEndBallPos.w);
	m_tParam[(int)TextureKind::Ball].rotate = easeOutBack(fTime, easeRotateTime, 0.0f, DirectX::XMConvertToRadians(1800.0f));

	if (fTime > easePosXTime)
	{
		fTime = 0.0f;
		m_bAnime[(int)AnimePhase::Ball] = true;
		return true;
	}
	fTime += 1.0f / fFPS;
	return false;
}

bool CSceneTitle::BatAnimation()
{
	constexpr float ce_fGroveMoveTime = 0.5f;
	constexpr float ce_fGroveMoveX = 200.0f;
	static float fTime = 0.0f;


	if (m_bAnime[(int)AnimePhase::BatGrove])
	{
		return true;
	}

	float fRad = fTime / ce_fGroveMoveTime * 180.0f;
	fRad = DirectX::XMConvertToRadians(fRad);
	float fCos = sinf(fRad);
	m_tParam[(int)TextureKind::Bat].pos.x = 
		(ce_fStartEndBatPos.z - ce_fStartEndBatPos.x) * (fTime / ce_fGroveMoveTime) +
		(fCos * ce_fGroveMoveX) + 
		ce_fStartEndBatPos.x;
	m_tParam[(int)TextureKind::Bat].pos.y =
		(ce_fStartEndBatPos.w - ce_fStartEndBatPos.y) *  (fTime / ce_fGroveMoveTime) +
		ce_fStartEndBatPos.y;
	m_tParam[(int)TextureKind::Bat].rotate = (0.0f - DirectX::XMConvertToRadians(1870.0f)) * (fTime / ce_fGroveMoveTime);

	m_tParam[(int)TextureKind::Bat2].pos.x = 
		(ce_fStartEndBatPos.z - ce_fStartEndBatPos.x) * (fTime / ce_fGroveMoveTime) +
		(fCos * -ce_fGroveMoveX) + 
		ce_fStartEndBatPos.x;
	m_tParam[(int)TextureKind::Bat2].pos.y = 
		(ce_fStartEndBatPos.w - ce_fStartEndBatPos.y) *  (fTime / ce_fGroveMoveTime) +
		ce_fStartEndBatPos.y;
	m_tParam[(int)TextureKind::Bat2].rotate = (0.0f - DirectX::XMConvertToRadians(-1870.0f)) * (fTime / ce_fGroveMoveTime);
	if (fTime > ce_fGroveMoveTime)
	{
		fTime = 0.0f;
		m_bAnime[(int)AnimePhase::BatGrove] = true;
		return true;
	}


	fTime += 1.0f / fFPS;
	return false;
}

bool CSceneTitle::LogoAnimation()
{
	static float fTime = 0.0f;
	constexpr float easeSizeTime = 0.5f;

	if (m_bAnime[(int)AnimePhase::Logo])
	{
		return true;
	}
	m_tParam[(int)TextureKind::Logo].size.x = easeOutBack(fTime, easeSizeTime, ce_fStartEndLogoSize.x, ce_fStartEndLogoSize.z);
	m_tParam[(int)TextureKind::Logo].size.y = easeOutBack(fTime, easeSizeTime, ce_fStartEndLogoSize.y, ce_fStartEndLogoSize.w);


	if (fTime > easeSizeTime)
	{
		fTime = 0.0f;
		m_bAnime[(int)AnimePhase::Logo] = true;
		return true;
	}

	fTime += 1.0f / fFPS;
	return false;
}

bool CSceneTitle::ButtonAnimation()
{
	static float fTime = 0.0f;
	constexpr float easePosTimeStartButton = 0.5f;
	constexpr float easePosTimeEndButton = 0.7f;
	constexpr float easePow = 2.5f;

	if (m_bAnime[(int)AnimePhase::Buttons])
	{
		return true;
	}

	m_tParam[(int)TextureKind::Start].pos.x = easeOutBack(fTime, easePosTimeStartButton, ce_fStartEndStartButtonPos.x, ce_fStartEndStartButtonPos.z, easePow);
	m_tParam[(int)TextureKind::Start].pos.y = easeOutBack(fTime, easePosTimeStartButton, ce_fStartEndStartButtonPos.y, ce_fStartEndStartButtonPos.w, easePow);

	m_tParam[(int)TextureKind::End].pos.x = easeOutBack(fTime, easePosTimeEndButton, ce_fStartEndEndButtonPos.x, ce_fStartEndEndButtonPos.z, easePow);
	m_tParam[(int)TextureKind::End].pos.y = easeOutBack(fTime, easePosTimeEndButton, ce_fStartEndEndButtonPos.y, ce_fStartEndEndButtonPos.w, easePow);

	if (fTime > easePosTimeEndButton)
	{
		fTime = 0.0f;
		m_bAnime[(int)AnimePhase::Buttons] = true;
		return true;
	}

	fTime += 1.0f / fFPS;
	return false;
}

void CSceneTitle::ResetSpriteParam()
{
	m_tParam[(int)TextureKind::Ball].pos = { ce_fStartEndBallPos.z ,ce_fStartEndBallPos.w };
	m_tParam[(int)TextureKind::Ball].rotate = 0.0f;
	m_tParam[(int)TextureKind::Bat].pos = { ce_fStartEndBatPos.z ,ce_fStartEndBatPos.w };
	m_tParam[(int)TextureKind::Bat].rotate = DirectX::XMConvertToRadians(1870.0f);
	m_tParam[(int)TextureKind::Bat2].pos = { ce_fStartEndBatPos.z ,ce_fStartEndBatPos.w };
	m_tParam[(int)TextureKind::Bat2].rotate = DirectX::XMConvertToRadians(-1870.0f);
	m_tParam[(int)TextureKind::Logo].size = { ce_fStartEndLogoSize.z ,ce_fStartEndLogoSize.w };
	m_tParam[(int)TextureKind::Start].pos = { ce_fStartEndStartButtonPos.z , ce_fStartEndStartButtonPos.w };
	m_tParam[(int)TextureKind::End].pos = { ce_fStartEndEndButtonPos.z , ce_fStartEndEndButtonPos.w };
}

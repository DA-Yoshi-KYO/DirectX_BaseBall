#include "SceneTitle.h"
#include "Input.h"
#include "Main.h"
#include "Sprite.h"
#include "Camera.h"

CSceneTitle::CSceneTitle()
	: m_pTexture{}, m_eTitlePhase(TitlePhase::Animation)
	, m_bSelected(false)
{
	for (int i = 0; i < (int)TextureKind::Max; i++)
	{
		m_pTexture[i] = std::make_unique<Texture>();
		m_tParam[i].pos = { 0.0f,0.0f };
		m_tParam[i].size = { 100.0f,100.0f };
		m_tParam[i].rotate = 0.0f;
		m_tParam[i].color = { 1.0f,1.0f,1.0f,1.0f };
		m_tParam[i].uvPos = { 0.0f,0.0f };
		m_tParam[i].uvSize = { 1.0f,1.0f };
		m_tParam[i].world = CCamera::Get2DWolrdMatrix();
		m_tParam[i].view = CCamera::Get2DViewMatrix();
		m_tParam[i].proj = CCamera::Get2DProjectionMatrix();
	}
	if (FAILED(m_pTexture[(int)TextureKind::Back]->Create("Assets/Texture/TitleBack.jpg"))) MessageBox(NULL, "Load failed SceneTitle.", "Error", MB_OK);
	m_tParam[(int)TextureKind::Back].size = { SCREEN_WIDTH,SCREEN_HEIGHT };

	if (FAILED(m_pTexture[(int)TextureKind::Logo]->Create("Assets/Texture/Ball.png"))) MessageBox(NULL, "Load failed SceneTitle.", "Error", MB_OK);


}

CSceneTitle::~CSceneTitle()
{

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
		Sprite::SetParam(m_tParam[i]);
		Sprite::SetTexture(m_pTexture[i].get());
		Sprite::Draw();
	}
}

void CSceneTitle::UpdateAnimation()
{
	if (IsKeyTrigger(VK_RETURN)) m_eTitlePhase = TitlePhase::Select;
}

void CSceneTitle::UpdateSelect()
{
	if (IsKeyTrigger(VK_RETURN) && !m_bSelected)
	{
		SetNext(SceneKind::Game); 
		m_bSelected = true;
	}
}

void CSceneTitle::ResetSpriteParam()
{
}

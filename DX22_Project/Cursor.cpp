#include "Cursor.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "Sprite.h"
#include "Main.h"
#include "Camera.h"
#include "Input.h"
#include "SceneGame.h"

CCursor::CCursor()
	: m_pTexture{ nullptr }, m_pStrikeZone(nullptr)
{
	m_pTexture[(int)CSceneGame::Playing::Attack] = std::make_unique<Texture>();
	m_tParam[(int)CSceneGame::Playing::Attack].pos = { 0.0f,-100.0f };
	m_tParam[(int)CSceneGame::Playing::Attack].size = DirectX::XMFLOAT2(50.0f, 50.0f);
	m_tParam[(int)CSceneGame::Playing::Attack].rotate = 0.0f;
	m_tParam[(int)CSceneGame::Playing::Attack].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tParam[(int)CSceneGame::Playing::Attack].uvPos = DirectX::XMFLOAT2(0.0f, 0.0f);
	m_tParam[(int)CSceneGame::Playing::Attack].uvSize = DirectX::XMFLOAT2(1.0f, 1.0f);
	m_tParam[(int)CSceneGame::Playing::Attack].world = CCamera::Get2DWolrdMatrix();
	m_tParam[(int)CSceneGame::Playing::Attack].view = CCamera::Get2DViewMatrix();
	m_tParam[(int)CSceneGame::Playing::Attack].proj = CCamera::Get2DProjectionMatrix();

	if (FAILED(m_pTexture[(int)CSceneGame::Playing::Attack]->Create(TEXPASS("Cursor.png")))) MessageBox(NULL, "Cursor.png", "Error", MB_OK);

	m_pTexture[(int)CSceneGame::Playing::Defence] = std::make_unique<Texture>();
	m_tParam[(int)CSceneGame::Playing::Defence].pos = { 0.0f,-100.0f };
	m_tParam[(int)CSceneGame::Playing::Defence].size = DirectX::XMFLOAT2(15.0f, 15.0f);
	m_tParam[(int)CSceneGame::Playing::Defence].rotate = 0.0f;
	m_tParam[(int)CSceneGame::Playing::Defence].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tParam[(int)CSceneGame::Playing::Defence].uvPos = DirectX::XMFLOAT2(0.0f, 0.0f);
	m_tParam[(int)CSceneGame::Playing::Defence].uvSize = DirectX::XMFLOAT2(1.0f, 1.0f);
	m_tParam[(int)CSceneGame::Playing::Defence].world = CCamera::Get2DWolrdMatrix();
	m_tParam[(int)CSceneGame::Playing::Defence].view = CCamera::Get2DViewMatrix();
	m_tParam[(int)CSceneGame::Playing::Defence].proj = CCamera::Get2DProjectionMatrix();

	if (FAILED(m_pTexture[(int)CSceneGame::Playing::Defence]->Create(TEXPASS("Ball.png")))) MessageBox(NULL, "Ball.png", "Error", MB_OK);
}

CCursor::~CCursor()
{
	m_pStrikeZone.release();
}

void CCursor::Update(int play)
{
	switch (play)
	{
		case (int)CSceneGame::Playing::Attack:
			if (IsKeyPress('D')) m_tParam[(int)CSceneGame::Playing::Attack].pos.x += 1.0f;
			if (IsKeyPress('A')) m_tParam[(int)CSceneGame::Playing::Attack].pos.x -= 1.0f;
			if (IsKeyPress('W')) m_tParam[(int)CSceneGame::Playing::Attack].pos.y += 1.0f;
			if (IsKeyPress('S')) m_tParam[(int)CSceneGame::Playing::Attack].pos.y -= 1.0f;

			if (m_tParam[(int)CSceneGame::Playing::Attack].pos.x >= m_pStrikeZone->GetPos().x + m_pStrikeZone->GetSize().x / 2.0f) m_tParam[(int)CSceneGame::Playing::Attack].pos.x = m_pStrikeZone->GetPos().x + m_pStrikeZone->GetSize().x / 2.0f;
			if (m_tParam[(int)CSceneGame::Playing::Attack].pos.x <= m_pStrikeZone->GetPos().x - m_pStrikeZone->GetSize().x / 2.0f) m_tParam[(int)CSceneGame::Playing::Attack].pos.x = m_pStrikeZone->GetPos().x - m_pStrikeZone->GetSize().x / 2.0f;
			if (m_tParam[(int)CSceneGame::Playing::Attack].pos.y >= m_pStrikeZone->GetPos().y + m_pStrikeZone->GetSize().y / 2.0f) m_tParam[(int)CSceneGame::Playing::Attack].pos.y = m_pStrikeZone->GetPos().y + m_pStrikeZone->GetSize().y / 2.0f;
			if (m_tParam[(int)CSceneGame::Playing::Attack].pos.y <= m_pStrikeZone->GetPos().y - m_pStrikeZone->GetSize().y / 2.0f) m_tParam[(int)CSceneGame::Playing::Attack].pos.y = m_pStrikeZone->GetPos().y - m_pStrikeZone->GetSize().y / 2.0f;
			break;
		case (int)CSceneGame::Playing::Defence:
			if (IsKeyPress(VK_RIGHT)) m_tParam[(int)CSceneGame::Playing::Defence].pos.x += 1.0f;
			if (IsKeyPress(VK_LEFT)) m_tParam[(int)CSceneGame::Playing::Defence].pos.x -= 1.0f;
			if (IsKeyPress(VK_UP)) m_tParam[(int)CSceneGame::Playing::Defence].pos.y += 1.0f;
			if (IsKeyPress(VK_DOWN)) m_tParam[(int)CSceneGame::Playing::Defence].pos.y -= 1.0f;

			if (m_tParam[(int)CSceneGame::Playing::Defence].pos.x >= m_pStrikeZone->GetPos().x + m_pStrikeZone->GetSize().x / 1.3f) m_tParam[(int)CSceneGame::Playing::Defence].pos.x = m_pStrikeZone->GetPos().x + m_pStrikeZone->GetSize().x / 1.3f;
			if (m_tParam[(int)CSceneGame::Playing::Defence].pos.x <= m_pStrikeZone->GetPos().x - m_pStrikeZone->GetSize().x / 1.3f) m_tParam[(int)CSceneGame::Playing::Defence].pos.x = m_pStrikeZone->GetPos().x - m_pStrikeZone->GetSize().x / 1.3f;
			if (m_tParam[(int)CSceneGame::Playing::Defence].pos.y >= m_pStrikeZone->GetPos().y + m_pStrikeZone->GetSize().y / 1.3f) m_tParam[(int)CSceneGame::Playing::Defence].pos.y = m_pStrikeZone->GetPos().y + m_pStrikeZone->GetSize().y / 1.3f;
			if (m_tParam[(int)CSceneGame::Playing::Defence].pos.y <= m_pStrikeZone->GetPos().y - m_pStrikeZone->GetSize().y / 1.3f) m_tParam[(int)CSceneGame::Playing::Defence].pos.y = m_pStrikeZone->GetPos().y - m_pStrikeZone->GetSize().y / 1.3f;
			break;
	default:
		break;
	}



}

void CCursor::Draw(int play)
{
	ImGui::SetNextWindowSize(ImVec2(320, 100));
	ImGui::Begin("hoge");
	ImGui::Text("fugafuga");
	ImGui::End();

	switch (play)
	{
	case (int)CSceneGame::Playing::Attack:
		SetRender2D();
		Sprite::SetParam(m_tParam[(int)CSceneGame::Playing::Attack]);
		Sprite::SetTexture(m_pTexture[(int)CSceneGame::Playing::Attack].get());
		Sprite::Draw();
		break;
	case (int)CSceneGame::Playing::Defence:
		Sprite::SetParam(m_tParam[(int)CSceneGame::Playing::Defence]);
		Sprite::SetTexture(m_pTexture[(int)CSceneGame::Playing::Defence].get());
		Sprite::Draw();
		break;
	default:
		break;
	}
}

void CCursor::SetStrikeZone(CStrikeZone* zone)
{
	m_pStrikeZone.reset(zone);
}

DirectX::XMFLOAT2 CCursor::GetPos(int play)
{
	switch (play)
	{
	case (int)CSceneGame::Playing::Attack:
		return m_tParam[(int)CSceneGame::Playing::Attack].pos;
		break;
	case (int)CSceneGame::Playing::Defence:
		return m_tParam[(int)CSceneGame::Playing::Defence].pos;
		break;
	default:
		break;
	}
}

DirectX::XMFLOAT2 CCursor::GetSize(int play)
{
	switch (play)
	{
	case (int)CSceneGame::Playing::Attack:
		return m_tParam[(int)CSceneGame::Playing::Attack].size;
		break;
	case (int)CSceneGame::Playing::Defence:
		return m_tParam[(int)CSceneGame::Playing::Defence].size;
		break;
	default:
		break;
	}
}

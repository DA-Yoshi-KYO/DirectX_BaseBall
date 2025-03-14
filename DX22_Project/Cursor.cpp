#include "Cursor.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "Sprite.h"
#include "Main.h"
#include "Camera.h"
#include "Input.h"

CCursor::CCursor()
	: m_pTexture(nullptr), m_pStrikeZone(nullptr)
{
	m_pTexture = std::make_unique<Texture>();
	m_tParam.pos = { 0.0f,-100.0f };
	m_tParam.size = DirectX::XMFLOAT2(50.0f, 50.0f);
	m_tParam.rotate = 0.0f;
	m_tParam.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tParam.uvPos = DirectX::XMFLOAT2(0.0f, 0.0f);
	m_tParam.uvSize = DirectX::XMFLOAT2(1.0f, 1.0f);
	m_tParam.world = CCamera::Get2DWolrdMatrix();
	m_tParam.view = CCamera::Get2DViewMatrix();
	m_tParam.proj = CCamera::Get2DProjectionMatrix();

	if (FAILED(m_pTexture->Create(TEXPASS("Cursor.png")))) MessageBox(NULL, "Cursor.png", "Error", MB_OK);
}

CCursor::~CCursor()
{
	m_pStrikeZone.release();
}

void CCursor::Update()
{
	if (IsKeyPress('D')) m_tParam.pos.x += 1.0f;
	if (IsKeyPress('A')) m_tParam.pos.x -= 1.0f;
	if (IsKeyPress('W')) m_tParam.pos.y += 1.0f;
	if (IsKeyPress('S')) m_tParam.pos.y -= 1.0f;

	if (m_tParam.pos.x >= m_pStrikeZone->GetPos().x + m_pStrikeZone->GetSize().x / 2.0f) m_tParam.pos.x = m_pStrikeZone->GetPos().x + m_pStrikeZone->GetSize().x / 2.0f;
	if (m_tParam.pos.x <= m_pStrikeZone->GetPos().x - m_pStrikeZone->GetSize().x / 2.0f) m_tParam.pos.x = m_pStrikeZone->GetPos().x - m_pStrikeZone->GetSize().x / 2.0f;
	if (m_tParam.pos.y >= m_pStrikeZone->GetPos().y + m_pStrikeZone->GetSize().y / 2.0f) m_tParam.pos.y = m_pStrikeZone->GetPos().y + m_pStrikeZone->GetSize().y / 2.0f;
	if (m_tParam.pos.y <= m_pStrikeZone->GetPos().y - m_pStrikeZone->GetSize().y / 2.0f) m_tParam.pos.y = m_pStrikeZone->GetPos().y - m_pStrikeZone->GetSize().y / 2.0f;

}

void CCursor::Draw()
{
	ImGui::SetNextWindowSize(ImVec2(320, 100));
	ImGui::Begin("hoge");
	ImGui::Text("fugafuga");
	ImGui::End();

	SetRender2D();
	Sprite::SetParam(m_tParam);
	Sprite::SetTexture(m_pTexture.get());
	Sprite::Draw();
}

void CCursor::SetStrikeZone(CStrikeZone* zone)
{
	m_pStrikeZone.reset(zone);
}

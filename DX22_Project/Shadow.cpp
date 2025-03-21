#include "Shadow.h"
#include "Sprite.h"
#include "Camera.h"
#include "SceneGame.h"

enum CollisionKind
{
	Object,
	Ground
};

CShadow::CShadow()
{
}

CShadow::~CShadow()
{
}

void CShadow::Update()
{
	Collision::Result result = Collision::Hit(m_Collision[Object], m_Collision[Ground] );
	if (result.isHit) 
	{
		
	}

	// ‰e‚ÌˆÊ’u‚ğŒvZ
	if (m_Collision[Ground].type == Collision::eBox) 
	{
		// ‚‚³‚ğl—¶‚¹‚¸A‚a‚‚˜‚Ì’†S‚Ü‚Å‚Ì‹——£‚ğ”»’è
		DirectX::XMFLOAT2 dist(
			fabsf(m_Collision[Object].box.center.x - m_Collision[Ground].box.center.x),
			fabsf(m_Collision[Object].box.center.z - m_Collision[Ground].box.center.z)
		);
		// ‚a‚‚˜“à‚Éû‚Ü‚Á‚Ä‚¢‚é‚©”»’è
		if (dist.x <= m_Collision[Ground].box.size.x * 0.5f &&
			dist.y <= m_Collision[Ground].box.size.z * 0.5f)
		{
			// ‚a‚‚˜‚Ì“V–Ê‚ÌÀ•W‚ğA‰e‚ÌˆÊ’u‚Éİ’è
			m_fShadowPos = {
				m_Collision[Object].box.center.x,
				m_Collision[Ground].box.center.y + m_Collision[Ground].box.size.y * 0.5f,
				m_Collision[Object].box.center.z };
		}
	}

}

void CShadow::Draw()
{
	// ‰e‚Ì‘å‚«‚³‚ğŒvZ
	float rate = (m_fObjectPos.y - m_fShadowPos.y) / METER(4.0f); // ‹——£‚ª‹ß‚¯‚ê‚Î0,‰“‚¯‚ê‚Î1
	float scale = (1.0f - rate); // rate‚ğ0‚È‚ç1A1‚È‚ç0‚É‚È‚é‚æ‚¤”½“]
	// ‰e‚ğ•\¦‚·‚é‚½‚ß‚Ìs—ñŒvZ
	DirectX::XMFLOAT4X4 mat;
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale, scale, scale); // scale‚ğŒ³‚ÉŠgk
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(90.0f)); // ƒXƒvƒ‰ƒCƒg‚ğ‰¡‚É“|‚·
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(
		m_fShadowPos.x, m_fShadowPos.y + CMETER(0.1f), m_fShadowPos.z);
	DirectX::XMMATRIX mWorld = S * R * T;
	
	mWorld = DirectX::XMMatrixTranspose(mWorld);

	CCamera* pCamera = CCamera::GetInstance(CSceneGame::GetCameraKind()).get();
	DirectX::XMStoreFloat4x4(&m_tParam.world, mWorld);
	m_tParam.view = pCamera->GetViewMatrix();
	m_tParam.proj = pCamera->GetProjectionMatrix();

	m_tParam.pos = { 0.0f,0.0f };
	m_tParam.size = { 100.0f,100.0f };
	m_tParam.uvPos = { 0.0f,0.0f };
	m_tParam.uvSize = { 1.0f,1.0f };
	m_tParam.color = { 0.0f, 0.0f, 0.0f, scale * 0.8f };

	Sprite::SetParam(m_tParam);
	Sprite::SetTexture(m_pTexture.get());
	Sprite::Draw();
}

void CShadow::SetObjectCollision(Collision::Info collision)
{
	m_Collision[0] = collision;
}

void CShadow::SetObjectPos(DirectX::XMFLOAT3 pos)
{
	m_fObjectPos = pos;
}

void CShadow::SetGroundCollision(Collision::Info collision)
{
	m_Collision[1] = collision;
}

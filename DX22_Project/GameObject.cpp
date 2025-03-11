#include "GameObject.h"

CGameObject::CGameObject()
	: m_pos(0.0f, 0.0f, 0.0f)
	, m_size(0.0f, 0.0f, 0.0f)
	, m_rotate(0.0f, 0.0f, 0.0f)
	, m_Collision{}
{

}

CGameObject::~CGameObject()
{

}

void CGameObject::SetModel(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 rotate, int ModelType)
{
}

void CGameObject::OnCollision(Collision::Result collision)
{
}

Collision::Info CGameObject::GetCollision()
{
	return m_Collision;
}
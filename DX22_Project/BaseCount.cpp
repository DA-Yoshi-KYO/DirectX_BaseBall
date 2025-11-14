#include "BaseCount.h"

constexpr int ce_nSheetSplit = 5;
constexpr DirectX::XMFLOAT3 ce_fBasePos = { 515.0f,210.0f,0.0f };
constexpr DirectX::XMFLOAT3 ce_fBaseSize = { 50.0f,40.0f,0.0f };
constexpr DirectX::XMFLOAT3 ce_fBaseAjust = { 42.0f,40.0f,0.0f };


CBaseCount::CBaseCount()
	: CCountObject()
{
	m_tParam.m_f2UVPos = { 0.0f / (float)ce_nSheetSplit,2.0f / (float)ce_nSheetSplit };
	m_tParam.m_f3Size = ce_fBaseSize;
}

CBaseCount::~CBaseCount()
{

}

void CBaseCount::Update()
{
	if (m_bActive) m_tParam.m_f4Color = DirectX::XMFLOAT4(1.0f,1.0f,0.0f,1.0f);
	else m_tParam.m_f4Color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);;

	CGameObject::Update();
}

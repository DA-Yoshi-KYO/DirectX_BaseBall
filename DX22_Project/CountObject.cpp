#include "CountObject.h"
#include "SpriteRenderer.h"

constexpr DirectX::XMFLOAT3 ce_fCountPos = { 395.0f,271.0f,0.0f };
constexpr DirectX::XMFLOAT3 ce_fCountSize = { 35.0f,35.0f,0.0f };
constexpr int ce_nSheetSplit = 5;

CCountObject::CCountObject()
	: CGameObject(), m_bActive(false)
{
	m_tParam.m_f3Size = ce_fCountSize;
	m_tParam.m_f2UVPos = { 1.0f / (float)ce_nSheetSplit,2.0f / (float)ce_nSheetSplit };
	m_tParam.m_f2UVSize = { 1.0f / (float)ce_nSheetSplit,1.0f / (float)ce_nSheetSplit };
}

CCountObject::~CCountObject()
{

}

void CCountObject::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("BallCountSheet.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite"));
}

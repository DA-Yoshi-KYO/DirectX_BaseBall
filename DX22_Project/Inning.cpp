#include "Inning.h"
#include "SpriteRenderer.h"

constexpr DirectX::XMFLOAT3 ce_fInningSize = { 50.0f,50.0f,0.0f };
// イニングのパラメータ
constexpr DirectX::XMFLOAT3 ce_fInningPos = { 395.0f,320.0f,0.0f };
constexpr DirectX::XMFLOAT3 ce_fInningAjust = { 30.0f,60.0f,0.0f };
// オモテ・ウラのパラメータ
constexpr DirectX::XMFLOAT3 ce_fTopBottomPos = { 435.0f,310.0f,0.0f };
constexpr DirectX::XMFLOAT3 ce_fTopBottomSize = { 50.0f,50.0f,0.0f };
constexpr DirectX::XMFLOAT3 ce_fTopBottomAjust = { 30.0f,60.0f,0.0f };
constexpr int ce_nSheetSplit = 5;

CInning::CInning()
	: m_nInningNum(1), m_bIsStart(true)
{

}

CInning::~CInning()
{

}

void CInning::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("BallCountSheet.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite"));

	m_tParam.m_f3Size = ce_fInningSize;
}

void CInning::Draw()
{
	// イニング数(1の位)の描画
	int nNum = m_nInningNum % 10;
	m_tParam.m_f3Pos = ce_fInningPos;
	m_tParam.m_f3Size = ce_fTopBottomSize;
	m_tParam.m_f2UVPos = { (float)(nNum % ce_nSheetSplit) / (float)ce_nSheetSplit ,(float)(nNum / ce_nSheetSplit) / (float)ce_nSheetSplit };
	CGameObject::Draw();

	// イニング数(10の位)の描画
	if (m_nInningNum >= 10)
	{
		nNum = m_nInningNum / 10;
		m_tParam.m_f3Pos = DirectX::XMFLOAT3(ce_fInningPos.x - ce_fInningAjust.x, ce_fInningPos.y, ce_fInningPos.z);
		m_tParam.m_f2UVPos = { (float)(nNum % ce_nSheetSplit) / (float)ce_nSheetSplit ,(float)(nNum / ce_nSheetSplit) / (float)ce_nSheetSplit };
		CGameObject::Draw();
	}

	// 表裏の描画
	m_tParam.m_f3Pos = ce_fTopBottomPos;
	m_tParam.m_f3Size = ce_fTopBottomSize;
	if (m_bIsStart) m_tParam.m_f2UVPos = { 2.0f / (float)ce_nSheetSplit ,2.0f / (float)ce_nSheetSplit };
	else m_tParam.m_f2UVPos = { 3.0f / (float)ce_nSheetSplit ,2.0f / (float)ce_nSheetSplit };
}

void CInning::InningProgress()
{
	if (!m_bIsStart)
	{
		m_nInningNum++;
	}

	m_bIsStart ^= true;
}

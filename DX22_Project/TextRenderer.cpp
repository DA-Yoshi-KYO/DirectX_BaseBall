#include "TextRenderer.h"
#include "DirectX.h"
#include "GameObject.h"

void CTextRenderer::Init()
{
	m_pWrite = nullptr;
	m_tFont = {};
	m_tParam = {};
	m_sText = "";
	m_wsText = L"";

	m_pWrite = new CWrite(&m_tFont);
	m_pWrite->Init(GetSwapChain());

	// フォントデータを改変
	m_tFont.m_fFontSize = 80;
	m_tFont.m_eFontThick = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	m_tFont.m_Color = D2D1::ColorF(D2D1::ColorF::Black);
	m_tFont.m_wsFont = m_pWrite->GetFontName(0);
	m_tFont.m_ShadowColor = D2D1::ColorF(D2D1::ColorF::White);
	m_tFont.m_ShadowOffset = D2D1::Point2F(5.0f, -5.0f);
	m_tFont.m_eTextAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;

	// フォントをセット
	m_pWrite->SetFont(m_tFont);
}

void CTextRenderer::Init(TextParam param)
{
	m_tParam = param;

	// フォントデータを改変
	m_tFont.m_fFontSize = m_tParam.m_fFontSize;
	m_tFont.m_wsFont = m_pWrite->GetFontName((int)m_tParam.m_eFontName);
	m_tFont.m_Color = D2D1::ColorF(m_tParam.m_Color.x, m_tParam.m_Color.y, m_tParam.m_Color.z, m_tParam.m_Color.w);
	m_tFont.m_ShadowColor = D2D1::ColorF(m_tParam.m_ShadowColor.x, m_tParam.m_ShadowColor.y, m_tParam.m_ShadowColor.z, m_tParam.m_ShadowColor.w);;
	m_tFont.m_ShadowOffset = m_tParam.m_ShadowOffSet;
	m_tFont.m_eTextAlignment = m_tParam.m_eAlignment;

	// フォントをセット
	m_pWrite->SetFont(m_tFont);
}

void CTextRenderer::Draw()
{
	DirectX::XMFLOAT3 center = m_pGameObject->GetPos();
	DirectX::XMFLOAT3 size = m_pGameObject->GetPos();

	D2D_RECT_F rect;
	rect.right = center.x + size.x * 0.5f;
	rect.left = center.x - size.x * 0.5f;
	rect.bottom = center.y + size.y * 0.5f;
	rect.top = center.y - size.y * 0.5f;

	switch (m_tParam.m_eStringKind)
	{
	case DrawStringKind::String:
		m_pWrite->DrawString(m_sText, rect, D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);
		break;
	case DrawStringKind::WString:
		m_pWrite->DrawWString(m_wsText, rect, D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);
		break;
	default:
		break;
	}
}

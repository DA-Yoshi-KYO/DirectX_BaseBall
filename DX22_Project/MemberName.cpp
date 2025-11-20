#include "MemberName.h"
#include "TextRenderer.h"

constexpr DirectX::XMFLOAT3 ce_fLineupSize = { 240.0f,45.0f,0.0f };

CMemberName::CMemberName()
{

}

CMemberName::~CMemberName()
{

}

void CMemberName::Init(std::wstring name)
{
	m_tParam.m_f3Size = ce_fLineupSize;

	TextParam text{};
	text.m_fFontSize = 30;
	text.m_Color = DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f);
	text.m_eFontName = FontName::LINESeedJP_TTF_Eb;
	text.m_ShadowColor = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	text.m_ShadowOffSet = D2D1::Point2F(5.0f, -5.0f);
	text.m_eAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
	text.m_eStringKind = DrawStringKind::WString;

	CTextRenderer* pRenderer = AddComponent<CTextRenderer>();
	pRenderer->Init(text);
	pRenderer->SetString(name);
}

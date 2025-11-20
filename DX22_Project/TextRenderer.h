#pragma once

#include "Component.h"
#include "Write.h"

enum class FontName
{
	LINESeedJP_TTF_Eb,
};

enum class DrawStringKind
{
	String,
	WString
};

struct TextParam
{
	float m_fFontSize;
	DirectX::XMFLOAT4 m_Color;
	DirectX::XMFLOAT4 m_ShadowColor;
	DWRITE_TEXT_ALIGNMENT m_eAlignment;
	FontName m_eFontName;
	D2D1_POINT_2F m_ShadowOffSet;
	DrawStringKind m_eStringKind;
};

class CTextRenderer : public CComponent
{
public:
	using CComponent::CComponent;
	void Init() override;
	void Init(TextParam param);
	void SetString(std::string text) { m_sText = text; }
	void SetString(std::wstring text) { m_wsText = text; }
	void Draw() override;

private:
	CWrite* m_pWrite;
	CWrite::FontData m_tFont;
	TextParam m_tParam;
	std::string m_sText;
	std::wstring m_wsText;

};

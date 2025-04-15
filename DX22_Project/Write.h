#pragma once

#include <d2d1.h>
#include <DWrite.h>
#include <DirectXMath.h>
#include <string>
#include <memory>
#include <vector>
#include <wrl.h>
#include "Defines.h"
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"Dwrite.lib")

//=============================================================================
//		フォントの保存場所
//=============================================================================
namespace FontList
{
	const std::wstring FontPath[] =
	{
		PATH_FONT("LINESeedJP_TTF_Eb.ttf"),
	};
}

class CustomFontCollectionLoader;

class CWrite
{
public:
	struct FontData
	{
		std::wstring m_wsFont;						// フォント名
		IDWriteFontCollection* m_pFontCollection;	// フォントコレクション
		DWRITE_FONT_WEIGHT m_eFontThick;			// フォントの太さ
		DWRITE_FONT_STYLE m_eFontStyle;				// フォントスタイル
		DWRITE_FONT_STRETCH m_eFontStretch;			// フォントの幅
		FLOAT m_fFontSize;							// フォントサイズ
		WCHAR const* m_wsLocaleName;					// ロケール名
		DWRITE_TEXT_ALIGNMENT m_eTextAlignment;		// テキストの配置
		D2D1_COLOR_F m_Color;							// フォントの色

		D2D1_COLOR_F m_ShadowColor;					// 影の色
		D2D1_POINT_2F m_ShadowOffset;					// 影のオフセット

		// デフォルト設定
		FontData()
		{
			m_wsFont = L"";
			m_pFontCollection = nullptr;
			m_eFontThick = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL;
			m_eFontStyle = DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL;
			m_eFontStretch = DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL;
			m_fFontSize = 20.0f;
			m_wsLocaleName = L"ja-jp";
			m_eTextAlignment = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING;
			m_Color = D2D1::ColorF(D2D1::ColorF::White);

			m_ShadowColor = D2D1::ColorF(D2D1::ColorF::Black);
			m_ShadowOffset = D2D1::Point2F(2.0f, -2.0f);
		}
	};
public:
	// FontDataの初期化
	CWrite(FontData* outFont);
	// DirectX関係の初期化
	HRESULT Init(IDXGISwapChain* swapChain);

	// フォントデータのセット
	HRESULT SetFont(FontData data);

	// 文字描画
	// string：文字列
	// pos：描画ポジション
	// options：テキストの整形
	HRESULT DrawString(std::string str, DirectX::XMFLOAT2 pos, D2D1_DRAW_TEXT_OPTIONS options, bool shadow = false);

	// 文字描画
	// string：文字列
	// rect：領域指定
	// options：テキストの整形
	HRESULT DrawString(std::string str, D2D1_RECT_F rect, D2D1_DRAW_TEXT_OPTIONS options, bool shadow = false);	

	// 文字描画
	// wstring：文字列
	// pos：描画ポジション
	// options：テキストの整形
	HRESULT DrawWString(std::wstring str, DirectX::XMFLOAT2 pos, D2D1_DRAW_TEXT_OPTIONS options, bool shadow = false);

	// 文字描画
	// wstring：文字列
	// rect：領域指定
	// options：テキストの整形
	HRESULT DrawWString(std::wstring str, D2D1_RECT_F rect, D2D1_DRAW_TEXT_OPTIONS options, bool shadow = false);	
	

	// フォント名を取得する
	std::wstring GetFontName(int num);

	// フォントのファイル名を取得する
	WCHAR* GetFontFileNameWithoutExtension(const std::wstring& filePath);
private:
	// ComオブジェクトはComPtrで定義する
	Microsoft::WRL::ComPtr <ID2D1Factory>			m_pD2DFactory = nullptr;	// Direct2Dリソース
	Microsoft::WRL::ComPtr <ID2D1RenderTarget>		m_pRenderTarget = nullptr;	// Direct2Dレンダーターゲット
	Microsoft::WRL::ComPtr <ID2D1SolidColorBrush>	m_pBrush = nullptr;			// Direct2Dブラシ設定
	Microsoft::WRL::ComPtr <ID2D1SolidColorBrush>	m_pShadowBrush = nullptr;	// Direct2Dブラシ設定（影）
	Microsoft::WRL::ComPtr <IDWriteFactory>			m_pDWriteFactory = nullptr;	// DirectWriteリソース
	Microsoft::WRL::ComPtr <IDWriteTextFormat>		m_pTextFormat = nullptr;	// DirectWriteテキスト形式
	Microsoft::WRL::ComPtr <IDWriteTextLayout>		m_pTextLayout = nullptr;	// DirectWriteテキスト書式
	Microsoft::WRL::ComPtr <IDXGISurface>			m_pBackBuffer = nullptr;	// サーフェス情報
	std::vector<Microsoft::WRL::ComPtr<IDWriteFontFile>> m_pFontFileList;		// フォントファイルリスト
	Microsoft::WRL::ComPtr <IDWriteFontCollection> m_pFontCollection = nullptr;// カスタムフォントコレクション
	// フォントデータ初期化用の構造体オブジェクト
	FontData m_tSetting = FontData();

	// フォント名のリスト
	std::vector<std::wstring> m_wsFontNameList;

	// stringをwstringへ変換する
	std::wstring StringToWString(std::string oString);

	// 指定されたパスのフォントを読み込む
	HRESULT FontLoader();

	// フォント名を取得し直す
	HRESULT GetFontFamilyName(IDWriteFontCollection* customFontCollection, const WCHAR* locale = L"en-us");

	// 全てのフォント名を取得し直す
	HRESULT GetAllFontFamilyName(IDWriteFontCollection* customFontCollection);
};
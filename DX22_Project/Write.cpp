#include "Write.h"

// フォントコレクションローダー
Microsoft::WRL::ComPtr <CustomFontCollectionLoader> g_pFontCollectionLoader = nullptr;
//=============================================================================
//		カスタムファイルローダー
//=============================================================================
class CustomFontFileEnumerator : public IDWriteFontFileEnumerator
{
public:
	CustomFontFileEnumerator(IDWriteFactory* factory, const std::vector<std::wstring>& fontFilePaths)
		: refCount_(0), factory_(factory), fontFilePaths_(fontFilePaths), currentFileIndex_(-1)
	{
		factory_->AddRef();
	}

	~CustomFontFileEnumerator()
	{
		factory_->Release();
	}

	IFACEMETHODIMP QueryInterface(REFIID iid, void** ppvObject) override
	{
		if (iid == __uuidof(IUnknown) || iid == __uuidof(IDWriteFontCollectionLoader))
		{
			*ppvObject = this;
			AddRef();
			return S_OK;
		}
		else
		{
			*ppvObject = nullptr;
			return E_NOINTERFACE;
		}
	}

	IFACEMETHODIMP_(ULONG) AddRef() override
	{
		return InterlockedIncrement(&refCount_);
	}

	IFACEMETHODIMP_(ULONG) Release() override
	{
		ULONG newCount = InterlockedDecrement(&refCount_);
		if (newCount == 0)
			delete this;

		return newCount;
	}

	IFACEMETHODIMP MoveNext(OUT BOOL* hasCurrentFile) override {
		if (++currentFileIndex_ < static_cast<int>(fontFilePaths_.size())) {
			*hasCurrentFile = TRUE;
			return S_OK;
		}
		else {
			*hasCurrentFile = FALSE;
			return S_OK;
		}
	}

	IFACEMETHODIMP GetCurrentFontFile(OUT IDWriteFontFile** fontFile) override
	{
		// フォントファイルを読み込む
		return factory_->CreateFontFileReference(fontFilePaths_[currentFileIndex_].c_str(), nullptr, fontFile);
	}

private:
	ULONG refCount_;

	// DirectWriteファクトリ
	IDWriteFactory* factory_;

	// フォントファイルのパス
	std::vector<std::wstring> fontFilePaths_;

	// 現在のファイルインデックス
	int currentFileIndex_;
};


//=============================================================================
//		カスタムフォントコレクションローダー
//=============================================================================
class CustomFontCollectionLoader : public IDWriteFontCollectionLoader
{
public:
	// コンストラクタ
	CustomFontCollectionLoader() : refCount_(0) {}

	// IUnknown メソッド
	IFACEMETHODIMP QueryInterface(REFIID iid, void** ppvObject) override
	{
		if (iid == __uuidof(IUnknown) || iid == __uuidof(IDWriteFontCollectionLoader))
		{
			*ppvObject = this;
			AddRef();
			return S_OK;
		}
		else
		{
			*ppvObject = nullptr;
			return E_NOINTERFACE;
		}
	}

	IFACEMETHODIMP_(ULONG) AddRef() override
	{
		return InterlockedIncrement(&refCount_);
	}

	IFACEMETHODIMP_(ULONG) Release() override
	{
		ULONG newCount = InterlockedDecrement(&refCount_);
		if (newCount == 0)
			delete this;

		return newCount;
	}

	// IDWriteFontCollectionLoader メソッド
	IFACEMETHODIMP CreateEnumeratorFromKey
	(
		IDWriteFactory* factory,
		void const* collectionKey,
		UINT32 collectionKeySize,
		OUT IDWriteFontFileEnumerator** fontFileEnumerator) override
	{
		// 読み込むフォントファイルのパスを渡す
		std::vector<std::wstring> fontFilePaths(std::begin(FontList::FontPath), std::end(FontList::FontPath));

		// カスタムフォントファイル列挙子の作成
		*fontFileEnumerator = new (std::nothrow) CustomFontFileEnumerator(factory, fontFilePaths);

		// メモリ不足の場合はエラーを返す
		if (*fontFileEnumerator == nullptr) { return E_OUTOFMEMORY; }

		return S_OK;
	}

private:
	ULONG refCount_;
};


CWrite::CWrite(FontData* outFont)
	: m_tSetting(*outFont)
{

}

HRESULT CWrite::Init(IDXGISwapChain* swapChain)
{
	HRESULT result = S_OK;

	// Direct2Dファクトリ情報の初期化
	result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_pD2DFactory.GetAddressOf());
	if (FAILED(result)) { return result; }

	// バックバッファの取得
	result = swapChain->GetBuffer(0, IID_PPV_ARGS(&m_pBackBuffer));
	if (FAILED(result)) { return result; }

	// dpiの設定
	FLOAT dpiX;
	FLOAT dpiY;
	m_pD2DFactory->GetDesktopDpi(&dpiX, &dpiY);

	// レンダーターゲットの作成
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), dpiX, dpiY);

	// サーフェスに描画するレンダーターゲットを作成
	result = m_pD2DFactory->CreateDxgiSurfaceRenderTarget(m_pBackBuffer.Get(), &props, m_pRenderTarget.GetAddressOf());
	if (FAILED(result)) { return result; }

	// アンチエイリアシングモードの設定
	m_pRenderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);

	// IDWriteFactoryの作成
	result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(m_pDWriteFactory.GetAddressOf()));
	if (FAILED(result)) { return result; }

	// カスタムフォントコレクションローダー
	g_pFontCollectionLoader = new CustomFontCollectionLoader();

	// カスタムフォントコレクションローダーの作成
	result = m_pDWriteFactory->RegisterFontCollectionLoader(g_pFontCollectionLoader.Get());
	if (FAILED(result)) { return result; }

	// フォントファイルの読み込み
	result = FontLoader();
	if (FAILED(result)) { return result; }

	// フォントを設定
	result = SetFont(m_tSetting);
	if (FAILED(result)) { return result; }

	return result;
}

HRESULT CWrite::SetFont(FontData data)
{
	HRESULT result = S_OK;

	// 設定をコピー
	m_tSetting = data;

	//関数CreateTextFormat()
	//第1引数：フォント名（L"メイリオ", L"Arial", L"Meiryo UI"等）
	//第2引数：フォントコレクション（nullptr）
	//第3引数：フォントの太さ（DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_WEIGHT_BOLD等）
	//第4引数：フォントスタイル（DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STYLE_OBLIQUE, DWRITE_FONT_STYLE_ITALIC）
	//第5引数：フォントの幅（DWRITE_FONT_STRETCH_NORMAL,DWRITE_FONT_STRETCH_EXTRA_EXPANDED等）
	//第6引数：フォントサイズ（20, 30等）
	//第7引数：ロケール名（L""）
	//第8引数：テキストフォーマット（&g_pTextFormat）
	result = m_pDWriteFactory->CreateTextFormat
	(
		GetFontFileNameWithoutExtension(m_tSetting.m_wsFont.c_str()),
		m_pFontCollection.Get(),
		m_tSetting.m_eFontThick,
		m_tSetting.m_eFontStyle,
		m_tSetting.m_eFontStretch,
		m_tSetting.m_fFontSize,
		m_tSetting.m_wsLocaleName,
		m_pTextFormat.GetAddressOf()
	);
	if (FAILED(result)) { return result; }

	//関数SetTextAlignment()
	//第1引数：テキストの配置（DWRITE_TEXT_ALIGNMENT_LEADING：前, DWRITE_TEXT_ALIGNMENT_TRAILING：後, DWRITE_TEXT_ALIGNMENT_CENTER：中央,
	//                         DWRITE_TEXT_ALIGNMENT_JUSTIFIED：行いっぱい）
	result = m_pTextFormat->SetTextAlignment(m_tSetting.m_eTextAlignment);
	if (FAILED(result)) { return result; }

	//関数CreateSolidColorBrush()
	//第1引数：フォント色（D2D1::ColorF(D2D1::ColorF::Black)：黒, D2D1::ColorF(D2D1::ColorF(0.0f, 0.2f, 0.9f, 1.0f))：RGBA指定）
	result = m_pRenderTarget->CreateSolidColorBrush(m_tSetting.m_Color, m_pBrush.GetAddressOf());
	if (FAILED(result)) { return result; }

	// 影用のブラシを作成
	result = m_pRenderTarget->CreateSolidColorBrush(m_tSetting.m_ShadowColor, m_pShadowBrush.GetAddressOf());
	if (FAILED(result)) { return result; }

	return result;
}

HRESULT CWrite::DrawString(std::string str, DirectX::XMFLOAT2 pos, D2D1_DRAW_TEXT_OPTIONS options, bool shadow)
{
	HRESULT result = S_OK;

	// 文字列の変換
	std::wstring wstr = StringToWString(str.c_str());

	// ターゲットサイズの取得
	D2D1_SIZE_F TargetSize = m_pRenderTarget->GetSize();

	// テキストレイアウトを作成
	result = m_pDWriteFactory->CreateTextLayout(wstr.c_str(), wstr.size(), m_pTextFormat.Get(), TargetSize.width, TargetSize.height,m_pTextLayout.GetAddressOf());
	if (FAILED(result)) { return result; }

	// 描画位置の確定
	D2D1_POINT_2F pounts;
	pounts.x = pos.x;
	pounts.y = pos.y + SCREEN_HEIGHT / 2.0f;

	// 描画の開始
	m_pRenderTarget->BeginDraw();

	// 影を描画する場合
	if (shadow)
	{
		// 影の描画
		m_pRenderTarget->DrawTextLayout(D2D1::Point2F(pounts.x - m_tSetting.m_ShadowOffset.x, pounts.y - m_tSetting.m_ShadowOffset.y),
			m_pTextLayout.Get(),
			m_pShadowBrush.Get(),
			options);
	}

	// 描画処理
	m_pRenderTarget->DrawTextLayout(pounts, m_pTextLayout.Get(), m_pBrush.Get(), options);

	// 描画の終了
	result = m_pRenderTarget->EndDraw();
	if (FAILED(result)) { return result; }

	return S_OK;
}

HRESULT CWrite::DrawString(std::string str, D2D1_RECT_F rect, D2D1_DRAW_TEXT_OPTIONS options, bool shadow)
{
	HRESULT result = S_OK;

	// 文字列の変換
	std::wstring wstr = StringToWString(str.c_str());

	// 描画の開始
	m_pRenderTarget->BeginDraw();

	if (shadow)
	{
		// 影の描画
		m_pRenderTarget->DrawText(wstr.c_str(),
			wstr.size(),
			m_pTextFormat.Get(),
			D2D1::RectF(rect.left - m_tSetting.m_ShadowOffset.x, rect.top - m_tSetting.m_ShadowOffset.y, rect.right - m_tSetting.m_ShadowOffset.x, rect.bottom - m_tSetting.m_ShadowOffset.y),
			m_pShadowBrush.Get(), options);
	}

	// 描画処理
	m_pRenderTarget->DrawText(wstr.c_str(), wstr.size(), m_pTextFormat.Get(), rect, m_pBrush.Get(), options);

	// 描画の終了
	result = m_pRenderTarget->EndDraw();
	if (FAILED(result)) { return result; }


	return S_OK;
}

HRESULT CWrite::DrawWString(std::wstring str, DirectX::XMFLOAT2 pos, D2D1_DRAW_TEXT_OPTIONS options, bool shadow)
{
	HRESULT result = S_OK;

	// 文字列の変換
	std::wstring wstr = str;

	// ターゲットサイズの取得
	D2D1_SIZE_F TargetSize = m_pRenderTarget->GetSize();

	// テキストレイアウトを作成
	result = m_pDWriteFactory->CreateTextLayout(wstr.c_str(), wstr.size(), m_pTextFormat.Get(), TargetSize.width, TargetSize.height, m_pTextLayout.GetAddressOf());
	if (FAILED(result)) { return result; }

	// 描画位置の確定
	D2D1_POINT_2F pounts;
	pounts.x = pos.x;
	pounts.y = pos.y + SCREEN_HEIGHT / 2.0f;

	// 描画の開始
	m_pRenderTarget->BeginDraw();

	// 影を描画する場合
	if (shadow)
	{
		// 影の描画
		m_pRenderTarget->DrawTextLayout(D2D1::Point2F(pounts.x - m_tSetting.m_ShadowOffset.x, pounts.y - m_tSetting.m_ShadowOffset.y),
			m_pTextLayout.Get(),
			m_pShadowBrush.Get(),
			options);
	}

	// 描画処理
	m_pRenderTarget->DrawTextLayout(pounts, m_pTextLayout.Get(), m_pBrush.Get(), options);

	// 描画の終了
	result = m_pRenderTarget->EndDraw();
	if (FAILED(result)) { return result; }

	return S_OK;
}

HRESULT CWrite::DrawWString(std::wstring str, D2D1_RECT_F rect, D2D1_DRAW_TEXT_OPTIONS options, bool shadow)
{
	HRESULT result = S_OK;

	// 文字列の変換
	std::wstring wstr = str;

	// 描画の開始
	m_pRenderTarget->BeginDraw();

	if (shadow)
	{
		// 影の描画
		m_pRenderTarget->DrawText(wstr.c_str(),
			wstr.size(),
			m_pTextFormat.Get(),
			D2D1::RectF(rect.left - m_tSetting.m_ShadowOffset.x, rect.top - m_tSetting.m_ShadowOffset.y, rect.right - m_tSetting.m_ShadowOffset.x, rect.bottom - m_tSetting.m_ShadowOffset.y),
			m_pShadowBrush.Get(), options);
	}

	// 描画処理
	m_pRenderTarget->DrawText(wstr.c_str(), wstr.size(), m_pTextFormat.Get(), rect, m_pBrush.Get(), options);

	// 描画の終了
	result = m_pRenderTarget->EndDraw();
	if (FAILED(result)) { return result; }


	return S_OK;
}

std::wstring CWrite::GetFontName(int num)
{
	// フォント名のリストが空だった場合
	if (m_wsFontNameList.empty())
	{
		return nullptr;
	}

	// リストのサイズを超えていた場合
	if (num >= static_cast<int>(m_wsFontNameList.size()))
	{
		return m_wsFontNameList[0];
	}

	return m_wsFontNameList[num];
}

WCHAR* CWrite::GetFontFileNameWithoutExtension(const std::wstring& filePath)
{
	// 末尾から検索してファイル名と拡張子の位置を取得
	size_t start = filePath.find_last_of(L"/\\") + 1;
	size_t end = filePath.find_last_of(L'.');

	// ファイル名を取得
	std::wstring fileNameWithoutExtension = filePath.substr(start, end - start).c_str();

	// 新しいWCHAR配列を作成
	WCHAR* fileName = new WCHAR[fileNameWithoutExtension.length() + 1];

	// 文字列をコピー
	wcscpy_s(fileName, fileNameWithoutExtension.length() + 1, fileNameWithoutExtension.c_str());

	// ファイル名を返す
	return fileName;
}

std::wstring CWrite::StringToWString(std::string oString)
{
	// SJIS → wstring
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, oString.c_str(), -1, (wchar_t*)NULL, 0);

	// バッファの取得
	wchar_t* cpUCS2 = new wchar_t[iBufferSize];

	// SJIS → wstring
	MultiByteToWideChar(CP_ACP, 0, oString.c_str(), -1, cpUCS2, iBufferSize);

	// stringの生成
	std::wstring oRet(cpUCS2, cpUCS2 + iBufferSize - 1);

	// バッファの破棄
	delete[] cpUCS2;

	// 変換結果を返す
	return oRet;
}

HRESULT CWrite::FontLoader()
{
	HRESULT result = S_OK;

	// カスタムフォントコレクションの作成
	result = m_pDWriteFactory->CreateCustomFontCollection
	(
		g_pFontCollectionLoader.Get(),
		m_pFontFileList.data(),
		m_pFontFileList.size(),
		&m_pFontCollection
	);
	if (FAILED(result)) { return result; }

	// フォント名を取得
	result = GetFontFamilyName(m_pFontCollection.Get());
	if (FAILED(result)) { return result; }

	return S_OK;
}

HRESULT CWrite::GetFontFamilyName(IDWriteFontCollection* customFontCollection, const WCHAR* locale)
{
	HRESULT result = S_OK;

	// フォントファミリー名一覧をリセット
	std::vector<std::wstring>().swap(m_wsFontNameList);

	// フォントの数を取得
	UINT32 familyCount = customFontCollection->GetFontFamilyCount();

	for (UINT32 i = 0; i < familyCount; i++)
	{
		// フォントファミリーの取得
		Microsoft::WRL::ComPtr <IDWriteFontFamily> fontFamily = nullptr;
		result = customFontCollection->GetFontFamily(i, fontFamily.GetAddressOf());
		if (FAILED(result)) { return result; }

		// フォントファミリー名の一覧を取得
		Microsoft::WRL::ComPtr <IDWriteLocalizedStrings> familyNames = nullptr;
		result = fontFamily->GetFamilyNames(familyNames.GetAddressOf());
		if (FAILED(result)) { return result; }

		// 指定されたロケールに対応するインデックスを検索
		UINT32 index = 0;
		BOOL exists = FALSE;
		result = familyNames->FindLocaleName(locale, &index, &exists);
		if (FAILED(result)) { return result; }

		// 指定されたロケールが見つからなかった場合は、デフォルトのロケールを使用
		if (!exists)
		{
			result = familyNames->FindLocaleName(L"en-us", &index, &exists);
			if (FAILED(result)) { return result; }
		}

		// フォントファミリー名の長さを取得
		UINT32 length = 0;
		result = familyNames->GetStringLength(index, &length);
		if (FAILED(result)) { return result; }

		// フォントファミリー名の取得
		WCHAR* name = new WCHAR[length + 1];
		result = familyNames->GetString(index, name, length + 1);
		if (FAILED(result)) { return result; }

		// フォントファミリー名を追加
		m_wsFontNameList.push_back(name);

		// フォントファミリー名の破棄
		delete[] name;
	}

	return result;
}

HRESULT CWrite::GetAllFontFamilyName(IDWriteFontCollection* customFontCollection)
{
	HRESULT result = S_OK;

	// フォントファミリー名一覧をリセット
	std::vector<std::wstring>().swap(m_wsFontNameList);

	// フォントファミリーの数を取得
	UINT32 familyCount = customFontCollection->GetFontFamilyCount();

	for (UINT32 i = 0; i < familyCount; i++)
	{
		// フォントファミリーの取得
		Microsoft::WRL::ComPtr <IDWriteFontFamily> fontFamily = nullptr;
		result = customFontCollection->GetFontFamily(i, fontFamily.GetAddressOf());
		if (FAILED(result)) { return result; }

		// フォントファミリー名の一覧を取得
		Microsoft::WRL::ComPtr <IDWriteLocalizedStrings> familyNames = nullptr;
		result = fontFamily->GetFamilyNames(familyNames.GetAddressOf());
		if (FAILED(result)) { return result; }

		// フォントファミリー名の数を取得
		UINT32 nameCount = familyNames->GetCount();

		// フォントファミリー名の数だけ繰り返す
		for (UINT32 j = 0; j < nameCount; ++j)
		{
			// フォントファミリー名の長さを取得
			UINT32 length = 0;
			result = familyNames->GetStringLength(j, &length);
			if (FAILED(result)) { return result; }

			// フォントファミリー名の取得
			WCHAR* name = new WCHAR[length + 1];
			result = familyNames->GetString(j, name, length + 1);
			if (FAILED(result)) { return result; }

			// フォントファミリー名を追加
			m_wsFontNameList.push_back(name);

			// フォントファミリー名の破棄
			delete[] name;
		}
	}

	return result;
}


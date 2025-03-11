// 実際に使用する環境でのヘッダーは適宜追加してください
#include "DirectX.h"
#include "Main.h"
#include "DirectWriteCustomFont.h"
#include "DirectWriteSample.h"

// フォントデータ
FontData data;

// DirectWrite描画クラス
DirectWriteCustomFont* Write;

int drawNum = 0;

void DirectWriteSample::Init()
{
	// DirectWrite用コンポーネントを作成
	Write = new DirectWriteCustomFont(&data);

	// 初期化
	Write->Init(GetSwapChain());


	// フォントデータを改変
	data.fontSize = 60;
	data.fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	data.Color = D2D1::ColorF(D2D1::ColorF::Red);
	data.font = Write->GetFontName(3);
	data.shadowColor = D2D1::ColorF(D2D1::ColorF::White);
	data.shadowOffset = D2D1::Point2F(5.0f, -5.0f);

	// フォントをセット
	Write->SetFont(data);
}

void DirectWriteSample::Uninit()
{
	// メモリ解放
	delete Write;
	Write = nullptr;
}

void DirectWriteSample::Update()
{
}

void DirectWriteSample::Draw()
{/*
	std::string checkkey = GetStringForKey();
	static std::string key = "";

	if (checkkey != "Error" && checkkey != "Back")
	{
		key += checkkey;
	}
	else if (checkkey == "Back" && key.size() != 0)
	{
		key.erase(key.size() - 1);
	}

	Write->DrawString(key, DirectX::XMFLOAT2(90, 90), D2D1_DRAW_TEXT_OPTIONS_NONE);

	Write->DrawString("描画位置は調整してください", DirectX::XMFLOAT2(90, 390), D2D1_DRAW_TEXT_OPTIONS_NONE);*/
}
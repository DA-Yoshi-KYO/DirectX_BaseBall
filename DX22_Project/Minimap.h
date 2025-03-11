#pragma once
#include "Texture.h" // レンダーターゲットも広義ではテクスチャ

class CMinimap
{
public:
	// コンストラクタ
	CMinimap();

	// デストラクタ
	~CMinimap();

	// ミニマップ表示
	void Draw();

	// ミニマップの作成開始
	void BeginRender();

	// ミニマップの作成終了
	void EndRender();

	// ミニマップの透明度を切り替え
	static void SwitchMinimapAplha();
private:
	RenderTarget* m_pRenderTarget; // 色情報書き込み先
	DepthStencil* m_pDepthStencil; // 深度情報書き込み先
	static float m_fAlpha; // 透明度
};
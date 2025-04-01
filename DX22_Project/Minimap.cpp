#include "Minimap.h"
#include "Sprite.h"
#include "Defines.h"

// 静的メンバ変数の初期化
float CMinimap::m_fAlpha = 1.0f;

CMinimap::CMinimap()
	// メンバ変数の初期化
	: m_pRenderTarget(nullptr), m_pDepthStencil(nullptr)
{
	// 作成する書き込み先のサイズ
	UINT width = 512;
	UINT height = 512;
	// レンダーターゲットの作成
	m_pRenderTarget = new RenderTarget();
	m_pRenderTarget->Create(DXGI_FORMAT_R8G8B8A8_UNORM, width, height);
	// 深度バッファの作成
	m_pDepthStencil = new DepthStencil();
	m_pDepthStencil->Create(width, height, false);
}

CMinimap::~CMinimap()
{
	// メモリの解放
	SAFE_DELETE(m_pDepthStencil);
	SAFE_DELETE(m_pRenderTarget);
}

void CMinimap::Draw()
{
	DirectX::XMFLOAT4X4 wvp[3];
	DirectX::XMMATRIX  mWorld, mView, mProj;

	// ワールド行列の作成
	mWorld =
		DirectX::XMMatrixScaling(1.0f, -1.0f, 1.0f) *
		DirectX::XMMatrixTranslation(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);
	mWorld = DirectX::XMMatrixTranspose(mWorld);
	DirectX::XMStoreFloat4x4(&wvp[0],mWorld);

	// ビュー行列の作成
	mView = DirectX::XMMatrixLookAtLH(
		DirectX::XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	mView = DirectX::XMMatrixTranspose(mView);
	DirectX::XMStoreFloat4x4(&wvp[1], mView);

	// プロジェクション行列の作成
	mProj = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.1f, 100.0f);
	mProj = DirectX::XMMatrixTranspose(mProj);
	DirectX::XMStoreFloat4x4(&wvp[2], mProj);

	// スプライトに各種情報を設定して表示
	Sprite::SetWorld(wvp[0]);
	Sprite::SetView(wvp[1]);
	Sprite::SetProjection(wvp[2]);
	Sprite::SetOffset({ 490.0f, 210.0f });
	Sprite::SetSize({ 200.0f, 200.0f }); // スプライトの表示サイズ(書き込みのサイズとは別)
	Sprite::SetColor({ 1,1,1,m_fAlpha });
	Sprite::SetTexture(m_pRenderTarget);
	Sprite::Draw();
}

void CMinimap::BeginRender()
{
	// 前回の書き込み情報をクリア
	m_pRenderTarget->Clear();
	m_pDepthStencil->Clear();
	// 描画先の変更
	SetRenderTargets(1, &m_pRenderTarget, m_pDepthStencil);
}

void CMinimap::EndRender()
{
	// デフォルトの描画先に戻す	
	RenderTarget* pDefRTV = GetDefaultRTV();
	DepthStencil* pDefDSV = GetDefaultDSV();
	SetRenderTargets(1, &pDefRTV, pDefDSV);
}

void CMinimap::SwitchMinimapAplha()
{
	// 透明度を切り替え
	m_fAlpha = (m_fAlpha == 1.0f) ? 0.5f : 1.0f;
}

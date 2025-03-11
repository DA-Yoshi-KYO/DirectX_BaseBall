#include "GaugeUI.h"
#include "Defines.h"

CGaugeUI::CGaugeUI()
    : m_pFrameTex(nullptr), m_pGaugeTex(nullptr), m_rate(0.1f) 
{
    // 枠部分のテクスチャを読み込み 
    m_pFrameTex = new Texture();
    if (FAILED(m_pFrameTex->Create(TEXPASS("UIFrame.png"))))
    {
        MessageBox(NULL, "Texture load failed.?nUI.cpp", "Error", MB_OK);
    }
    // ゲージ部分のテクスチャを読み込み 
    m_pGaugeTex = new Texture();
    if (FAILED(m_pGaugeTex->Create(TEXPASS("UIGauge.png")))) {
        MessageBox(NULL, "Texture load failed.?nUI.cpp", "Error", MB_OK);
    }
    m_pos = { 640.0f  ,650.0f };
}

CGaugeUI::~CGaugeUI()
{
    if (m_pFrameTex)
    {
        delete m_pFrameTex;
        m_pFrameTex = nullptr;
    }
    if (m_pGaugeTex)
    {
        delete m_pGaugeTex;
        m_pGaugeTex = nullptr;
    }
}

void CGaugeUI::Update()
{

}

void CGaugeUI::Draw()
{
    // スプライトの表示に必要な行列を計算 
    DirectX::XMFLOAT4X4 world, view, proj;
    DirectX::XMMATRIX mView = DirectX::XMMatrixLookAtLH(
            DirectX::XMVectorSet(0.0f, 0.0f, -0.3f, 0.0f),
            DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
            DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
    DirectX::XMMATRIX mProj = DirectX::XMMatrixOrthographicOffCenterLH(
        0.0f,SCREEN_WIDTH,SCREEN_HEIGHT,0.0f,0.1f,10.0f);
    mView = DirectX::XMMatrixTranspose(mView);
    mProj = DirectX::XMMatrixTranspose(mProj);
    DirectX::XMStoreFloat4x4(&view, mView);
    DirectX::XMStoreFloat4x4(&proj, mProj);

    Sprite::SetView(view);
    Sprite::SetProjection(proj);

    // フレームとゲージの描画 
    DirectX::XMFLOAT2 pos = m_pos;
    DirectX::XMFLOAT2 size[] = {{210.0f, 40.0f}, {190.0f, 20.0f}};

    Texture* pTex[] = { m_pFrameTex, m_pGaugeTex };
    for (int i = 0; i < 2; ++i) {
        DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(pos.x - size[i].x * 0.5f, pos.y, 0.0f);
        DirectX::XMMATRIX S;
        if (i == 0) S = DirectX::XMMatrixScaling(1.0f, -1.0f, 1.0f);// フレームは倍率を変更せずに表示 
        else S = DirectX::XMMatrixScaling(m_rate, -1.0f, 1.0f); // ゲージは横方向をm_rateに応じて拡縮 
        DirectX::XMMATRIX mWorld = S * T;
        mWorld = DirectX::XMMatrixTranspose(mWorld);
        DirectX::XMStoreFloat4x4(&world, mWorld);
        Sprite::SetWorld(world);       // スプライトのワールド行列を設定 
        Sprite::SetSize(size[i]);      // スプライトのサイズを設定 
        Sprite::SetOffset({ size[i].x * 0.5f, 0.0f }); // スプライトの原点を変更 
        Sprite::SetColor({1.0f, 1.0f, 1.0f, 1.0f});      // テクスチャを設定 
        Sprite::SetTexture(pTex[i]);      // テクスチャを設定 
        Sprite::Draw();
    }
    Sprite::SetOffset({ 0.0f, 0.0f });
}

void CGaugeUI::SetGauge(float rate)
{
    m_rate = rate;
}
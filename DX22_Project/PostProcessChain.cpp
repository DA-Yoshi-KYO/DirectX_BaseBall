#include "PostProcessChain.h"
#include "PostProcess.h"
#include "Texture.h"
#include "Defines.h"
#include "Sprite.h"
#include <typeinfo>

CPostProcessChain* CPostProcessChain::m_pInstance = nullptr;

CPostProcessChain::CPostProcessChain()
    : m_pScreenRTV(nullptr), m_pOutRTV(nullptr)
    , m_pPostProcessList{}
{
    // これまでの描画結果
    m_pScreenRTV = new RenderTarget();
    m_pScreenRTV->Create(DXGI_FORMAT_R8G8B8A8_UNORM, SCREEN_WIDTH, SCREEN_HEIGHT);

    // チェイン用の描画先
    m_pOutRTV = new RenderTarget();
    m_pOutRTV->Create(DXGI_FORMAT_R8G8B8A8_UNORM, SCREEN_WIDTH, SCREEN_HEIGHT);

#if 1
    float clear[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    m_pScreenRTV->Clear(clear);
    m_pOutRTV->Clear(clear);
#endif
}

void CPostProcessChain::Attach()
{
    // アクティブなポストプロセスを探索
    m_pActivePostProcessList.clear();
    for (int i = 0; i < m_pPostProcessList.size(); i++)
    {
        if (m_pPostProcessList[i]->GetIsActive()) m_pActivePostProcessList.push_back(m_pPostProcessList[i]);
    }

    if (!m_pActivePostProcessList.empty())
    {
        RenderTarget* pScreen = nullptr;    // これまでの描画結果
        RenderTarget* pOut = nullptr;       // チェイン用の描画先

        // アクティブ状態のポストプロセスのみ処理を行う
        for (int i = 0; i < m_pActivePostProcessList.size(); i++)
        {
            //---ポストプロセスのチェイン処理
            // ①,それまでの描画結果を受け取る
            // ②,最後のポストプロセスでなければ、チェイン用の描画先に描画する
            // ③,その描画結果をまた受け取り、今度はそれまでの描画で使っていたスクリーンを描画先とする(スワップ)
            // ④,②③を繰り返し、最後のポストプロセスになった際にデフォルトの描画先を使用する

            // スワップ処理
            if (i % 2 == 0)
            {
                pOut = m_pOutRTV;
                pScreen = m_pScreenRTV;
            }
            else
            {
                pOut = m_pScreenRTV;
                pScreen = m_pOutRTV;
            }

            // 描画先の変更
            bool isLast = (i == m_pActivePostProcessList.size() - 1);
            float clear[] = { 1.0f,1.0f,1.0f,1.0f };
            if (isLast)
            {
                RenderTarget* pDefaultRTV = GetDefaultRTV();
                pDefaultRTV->Clear(clear);
                SetRenderTargets(1, &pDefaultRTV, nullptr);
            }
            else
            {
                pOut->Clear(clear);
                SetRenderTargets(1, &pOut, nullptr);
            }

            // スプライトの表示
            SetMatrix();
            m_pActivePostProcessList[i]->Write(); // バッファへの書き込み
            Sprite::SetPixelShader(m_pActivePostProcessList[i]->GetPS());
            // これまでの描画結果をテクスチャとして使用
            Sprite::SetTexture(pScreen);   
            Sprite::Draw();
        }
    }
    else
    {
        // ポストプロセスを使用しない場合、デフォルトのレンダーターゲットにそのまま描画する
        RenderTarget* pDefaultRTV = GetDefaultRTV();
        SetRenderTargets(1, &pDefaultRTV, nullptr);

        // スプライトの表示
        SetMatrix();
        Sprite::SetPixelShader(nullptr);
        Sprite::SetTexture(m_pScreenRTV);   // これまでの描画結果をテクスチャとして使用
        Sprite::Draw();
    }
}

CPostProcessChain* CPostProcessChain::GetInstance()
{
    if (!m_pInstance)
    {
        m_pInstance = new CPostProcessChain();
    }

    return m_pInstance;
}

RenderTarget* CPostProcessChain::GetScreenTarget()
{
    return m_pScreenRTV;
}

void CPostProcessChain::Release()
{
    SAFE_DELETE(m_pScreenRTV);
    SAFE_DELETE(m_pOutRTV);
    SAFE_DELETE(m_pInstance);
}

void CPostProcessChain::ClearPostProcessList()
{
    for (auto itr : m_pPostProcessList)
    {
        SAFE_DELETE(itr);
    }
}

void CPostProcessChain::SetMatrix()
{
    DirectX::XMFLOAT4X4 world;
    DirectX::XMStoreFloat4x4(&world, DirectX::XMMatrixIdentity());

    DirectX::XMFLOAT4X4 view;
    DirectX::XMStoreFloat4x4(&view,
        DirectX::XMMatrixTranspose(
            DirectX::XMMatrixLookAtLH(
                DirectX::XMVectorSet(0.0f, 0.0f, -10.0f, 0.0f),
                DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
                DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
            )
        )
    );

    DirectX::XMFLOAT4X4 proj;
    DirectX::XMStoreFloat4x4(&proj,
        DirectX::XMMatrixTranspose(
            DirectX::XMMatrixOrthographicLH(1.0f, 1.0f, 0.1f, 100.0f
            )
        )
    );

    Sprite::SetWorld(world);
    Sprite::SetView(view);
    Sprite::SetProjection(proj);
}

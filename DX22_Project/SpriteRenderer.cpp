// インクルード部
#include "SpriteRenderer.h"
#include "Sprite.h"
#include "DirectX.h"
#include "Defines.h"
#include "PostProcessChain.h"

CSpriteRenderer::~CSpriteRenderer()
{

}

void CSpriteRenderer::Draw()
{
    // キーが設定されていない時は描画しない
    if (m_sTextureKey.empty()) return;

    // 深度バッファを無効にする 
    //RenderTarget* pRTV = CPostProcessChain::GetInstance()->GetScreenTarget();
    RenderTarget* pRTV =GetDefaultRTV();
    SetRenderTargets(1, &pRTV, nullptr);

    // カリングのセット
    //SetCullingMode(m_tParam.m_eCulling);
    // 描画用パラメータのセット
    Sprite::SetOffset(DirectX::XMFLOAT2());
    Sprite::SetSize(DirectX::XMFLOAT2(m_tParam.m_f3Size.x, m_tParam.m_f3Size.y));
    Sprite::SetUVPos(m_tParam.m_f2UVPos);
    Sprite::SetUVScale(m_tParam.m_f2UVSize);
    Sprite::SetColor(m_tParam.m_f4Color);

    DirectX::XMMATRIX mWorld =
        DirectX::XMMatrixRotationRollPitchYaw(m_tParam.m_f3Rotate.x, m_tParam.m_f3Rotate.y, m_tParam.m_f3Rotate.z) *
        DirectX::XMMatrixTranslation(m_tParam.m_f3Pos.x, m_tParam.m_f3Pos.y, m_tParam.m_f3Pos.z);
    mWorld = DirectX::XMMatrixTranspose(mWorld);
    DirectX::XMFLOAT4X4 world;
    DirectX::XMStoreFloat4x4(&world, mWorld);
    Sprite::SetWorld(world);

    DirectX::XMFLOAT4X4 view;
    DirectX::XMMATRIX mView = DirectX::XMMatrixLookAtLH(
        DirectX::XMVectorSet(0.0f, 0.0f, -0.3f, 0.0f),
        DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
        DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
    mView = DirectX::XMMatrixTranspose(mView);
    DirectX::XMStoreFloat4x4(&view, mView);
    Sprite::SetView(view);

    DirectX::XMMATRIX mProj =
        DirectX::XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.1f, 10.0f);
    mProj = DirectX::XMMatrixTranspose(mProj);
    DirectX::XMFLOAT4X4 proj;
    DirectX::XMStoreFloat4x4(&proj, mProj);
    Sprite::SetProjection(proj);

    // シェーダーのセット
    Sprite::SetVertexShader(nullptr);
    Sprite::SetPixelShader(nullptr);

    // テクスチャのセット
    Sprite::SetTexture(m_pTextureList[m_sTextureKey]);

    // 描画
    Sprite::Draw();
}

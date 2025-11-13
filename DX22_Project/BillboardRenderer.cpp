// インクルード部
#include "BillboardRenderer.h"
#include "Sprite.h"
#include "DirectX.h"
#include "CameraManager.h"
#include "PostProcessChain.h"

CBillboardRenderer::~CBillboardRenderer()
{

}

void CBillboardRenderer::Draw()
{
    // キーが設定されていない時は描画しない
    if (m_sTextureKey.empty()) return;

    // フラグによって深度バッファを使用するか決める
    RenderTarget* pRTV = CPostProcessChain::GetInstance()->GetScreenTarget();
    DepthStencil* pDSV = GetDefaultDSV();
    if (m_bIsDepth) SetRenderTargets(1, &pRTV, pDSV);
    else SetRenderTargets(1, &pRTV, nullptr);

    // カリングのセット
    SetCullingMode(m_tParam.m_eCulling);

    // 描画用パラメータのセット
    Sprite::SetOffset(DirectX::XMFLOAT2());
    Sprite::SetSize(DirectX::XMFLOAT2(m_tParam.m_f3Size.x, m_tParam.m_f3Size.y));
    Sprite::SetUVPos(m_tParam.m_f2UVPos);
    Sprite::SetUVScale(m_tParam.m_f2UVSize);
    Sprite::SetColor(m_tParam.m_f4Color);

    CCameraBase* pCamera = CCameraManager::GetInstance()->GetMainCamera();

    DirectX::XMMATRIX mCamInv = DirectX::XMMatrixIdentity();
    DirectX::XMFLOAT4X4 view = pCamera->GetViewMatrix(false);
    mCamInv = DirectX::XMLoadFloat4x4(&view);
    mCamInv = DirectX::XMMatrixInverse(nullptr, mCamInv);
    DirectX::XMStoreFloat4x4(&view, mCamInv);
    view._41 = view._42 = view._43 = 0.0f;
    mCamInv = DirectX::XMLoadFloat4x4(&view);
    DirectX::XMMATRIX mWorld =
        mCamInv *
        DirectX::XMMatrixTranslation(m_tParam.m_f3Pos.x, m_tParam.m_f3Pos.y, m_tParam.m_f3Pos.z);
    mWorld = DirectX::XMMatrixTranspose(mWorld);
    DirectX::XMFLOAT4X4 world;
    DirectX::XMStoreFloat4x4(&world, mWorld);
    Sprite::SetWorld(world);

    Sprite::SetView(pCamera->GetViewMatrix());

    Sprite::SetProjection(pCamera->GetProjectionMatrix());

    // シェーダーのセット
    Sprite::SetVertexShader(m_pVSList[m_sVSKey]);
    Sprite::SetPixelShader(m_pPSList[m_sPSKey]);

    // テクスチャのセット
    Sprite::SetTexture(m_pTextureList[m_sTextureKey]);

    // 描画
    Sprite::Draw();
}

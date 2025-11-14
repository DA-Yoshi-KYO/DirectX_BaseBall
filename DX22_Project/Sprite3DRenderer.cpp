// インクルード部
#include "Sprite3DRenderer.h"
#include "Sprite.h"
#include "DirectX.h"
#include "Defines.h"
#include "Camera.h"
#include "PostProcessChain.h"

CSprite3DRenderer::~CSprite3DRenderer()
{

}

void CSprite3DRenderer::Draw()
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

    DirectX::XMMATRIX mWorld =
        DirectX::XMMatrixRotationRollPitchYaw(m_tParam.m_f3Rotate.x, m_tParam.m_f3Rotate.y, m_tParam.m_f3Rotate.z) *
        DirectX::XMMatrixTranslation(m_tParam.m_f3Pos.x, m_tParam.m_f3Pos.y, m_tParam.m_f3Pos.z);
    mWorld = DirectX::XMMatrixTranspose(mWorld);
    DirectX::XMFLOAT4X4 world;
    DirectX::XMStoreFloat4x4(&world, mWorld);
    Sprite::SetWorld(world);

    CCamera* pCamera = CCamera::GetInstance();

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

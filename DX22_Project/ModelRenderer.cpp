// インクルード部
#include "ModelRenderer.h"
#include "SpriteRendererBase.h"
#include "CameraManager.h"
#include "PostProcessChain.h"

std::map<std::string, Model*> CModelRenderer::m_pModelList = {};

CModelRenderer::~CModelRenderer()
{

}

void CModelRenderer::Init()
{

}

void CModelRenderer::Draw()
{
    // キーが設定されていない時は描画しない
    if (m_sModelKey.empty()) return;

    RenderTarget* pRTV = CPostProcessChain::GetInstance()->GetScreenTarget();
    DepthStencil* pDSV = GetDefaultDSV();
    SetRenderTargets(1, &pRTV, pDSV);
    
    DirectX::XMFLOAT4X4 wvp[3];
    DirectX::XMMATRIX world =
        DirectX::XMMatrixScaling(m_tParam.m_f3Size.x, m_tParam.m_f3Size.y, m_tParam.m_f3Size.z) *
        DirectX::XMMatrixRotationRollPitchYaw(m_tParam.m_f3Rotate.x, m_tParam.m_f3Rotate.y, m_tParam.m_f3Rotate.z) *
        DirectX::XMMatrixTranslation(m_tParam.m_f3Pos.x, m_tParam.m_f3Pos.y, m_tParam.m_f3Pos.z);
    world = DirectX::XMMatrixTranspose(world);
    DirectX::XMStoreFloat4x4(&wvp[0], world);

    CCameraBase* pCamera = CCameraManager::GetInstance()->GetMainCamera();
    wvp[1] = pCamera->GetViewMatrix();

    wvp[2] = pCamera->GetProjectionMatrix();

    // モデルにシェーダーをセット
    Model* pModel = m_pModelList[m_sModelKey];
    VertexShader* pVS = m_pVSList[m_sVSKey];
    PixelShader* pPS = m_pPSList[m_sPSKey];

    pPS->WriteBuffer(0, &m_tParam.m_f4Color);
    pVS->WriteBuffer(0, wvp);

    pModel->SetPixelShader(pPS);
    pModel->SetVertexShader(pVS);
    
    // モデルの描画
    for (unsigned int i = 0; i < pModel->GetMeshNum(); i++)
    {
        Texture* tex = pModel->GetMaterial(i)->pTexture;
        if (tex)pPS->SetTexture(0, tex);
        if (pModel) pModel->Draw(i);
    }
}

void CModelRenderer::Load(std::string inPath, float inScale, Model::Flip inFlip)
{
    m_sModelKey = inPath;

    auto itr = m_pModelList.find(m_sModelKey);
    if (itr != m_pModelList.end())
    {
        return;
    }

    Model* pModel = new Model();
    if (!pModel->Load(m_sModelKey.c_str(), inScale, inFlip))
    {
        MessageBox(NULL, m_sModelKey.c_str(), "Load failed.", MB_OK);
    }

    m_pModelList[m_sModelKey] = pModel;
}

void CModelRenderer::UnloadAll()
{
    for (auto itr : m_pModelList)
    {
        if (itr.second)
        {
            delete itr.second;
            itr.second = nullptr;
        }
    }
}

Model* CModelRenderer::GetModel()
{
    return m_pModelList[m_sModelKey];
}

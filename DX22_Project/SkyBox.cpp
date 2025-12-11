// インクルード部
#include "SkyBox.h"
#include "Camera.h"
#include "ModelRenderer.h"

CSkyBox::CSkyBox()
    : CGameObject()
{
    m_tParam.m_f3Size = DirectX::XMFLOAT3(100.0f, 100.0f, 100.0f);
}

CSkyBox::~CSkyBox()
{

}

void CSkyBox::Init()
{
    // 
    // コンポーネントの追加
    CModelRenderer* pRenderer = AddComponent<CModelRenderer>();
    //  描画に使用するモデルのキーをセット
    pRenderer->Load(PATH_MODEL("SkyBox.obj"));
    pRenderer->LoadVertexShader(PATH_SHADER("VS_Object.cso"));
    pRenderer->LoadPixelShader(PATH_SHADER("PS_TexColor.cso"));
    pRenderer->LoadTexture(PATH_MODEL("Epic_BlueSunset_EquiRect_flat.png"));
    // 表明をカリングする(裏面を描画する)
    m_tParam.m_eCulling = D3D11_CULL_FRONT;
}

void CSkyBox::Update()
{
    // 座標の更新
    m_tParam.m_f3Pos = CCamera::GetInstance()->GetPos();

    CGameObject::Update();
}

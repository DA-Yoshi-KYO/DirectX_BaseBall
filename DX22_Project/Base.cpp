#include "Base.h"
#include "ModelRenderer.h"

constexpr DirectX::XMFLOAT3 ce_fPitcherPos = { WORLD_AJUST, WORLD_AJUST - 8.0f, WORLD_AJUST + 65.0f };	// グラウンドにいるピッチャーの位置(基準値)

CBase::CBase()
{

}

CBase::~CBase()
{

}

void CBase::Init()
{

}

void CBase::Init(BaseKind kind)
{
    CModelRenderer* pRenderer = AddComponent<CModelRenderer>();

    switch (kind)
    {
    case BaseKind::Home:
        m_tParam.m_f3Pos = { ce_fPitcherPos.x, 0.0f, ce_fPitcherPos.z + 75.0f };
        m_tParam.m_f3Size = { 1.5f,0.5f,1.5f };
        m_tParam.m_f3Rotate = { 0.0f,0.0f,0.0f };
       
        pRenderer->Load(PATH_MODEL("HomeBase.obj"));
        break;
    case BaseKind::First:
       m_tParam.m_f3Pos = { ce_fPitcherPos.x - 70.0f,0.0f,ce_fPitcherPos.z };
       m_tParam.m_f3Rotate = { 0.0f,DirectX::XMConvertToRadians(45.0f),0.0f };
       m_tParam.m_f3Size = { 3.0f,3.0f,3.0f };
       
       pRenderer->Load(PATH_MODEL("base.obj"));
        break;
    case BaseKind::Second:
       m_tParam.m_f3Pos = { ce_fPitcherPos.x ,0.0f,ce_fPitcherPos.z - 75.0f };
       m_tParam.m_f3Size = { 3.0f,3.0f,3.0f };
       m_tParam.m_f3Rotate = { 0.0f,DirectX::XMConvertToRadians(45.0f),0.0f };
        
       pRenderer->Load(PATH_MODEL("base.obj"));
        break;
    case BaseKind::Third:
       m_tParam.m_f3Pos = { ce_fPitcherPos.x + 70.0f, 0.0f,ce_fPitcherPos.z };
       m_tParam.m_f3Size = { 3.0f,3.0f,3.0f };
       m_tParam.m_f3Rotate = { 0.0f,DirectX::XMConvertToRadians(45.0f),0.0f };

        pRenderer->Load(PATH_MODEL("base.obj"));
        break;
    default:
        break;
    }
}

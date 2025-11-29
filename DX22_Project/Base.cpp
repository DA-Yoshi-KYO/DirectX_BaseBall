#include "Base.h"
#include "ModelRenderer.h"
#include "CollisionBox.h"

constexpr DirectX::XMFLOAT3 ce_fPitcherPos = { WORLD_AJUST, WORLD_AJUST - 8.0f, WORLD_AJUST + 65.0f };	// グラウンドにいるピッチャーの位置(基準値)

CBase::CBase()
    : CGameObject()
    , m_eBaseKind(BaseKind::First), m_bIsBaseCovered(false)
{

}

CBase::~CBase()
{

}

void CBase::Init()
{
    AddComponent<CCollisionBox>();
}

void CBase::Init(BaseKind kind)
{
   // CModelRenderer* pRenderer = AddComponent<CModelRenderer>();
    m_eBaseKind = kind;

    std::string baseName = "";
    switch (m_eBaseKind)
    {
    case BaseKind::Home:
        m_tParam.m_f3Pos = { 0.0f, -7.0f, -218.0f };
        m_tParam.m_f3Size = { 6.0f,1.0f,6.0f };
        m_tParam.m_f3Rotate = { 0.0f,0.0f,0.0f };
        baseName = "HomeBase";
       // pRenderer->Load(PATH_MODEL("HomeBase.obj"));
        break;
    case BaseKind::First:
       m_tParam.m_f3Pos = { 70.0f,-7.0f,-146.0f };
       m_tParam.m_f3Size = { 6.0f,1.0f,6.0f };
       m_tParam.m_f3Rotate = { 0.0f,DirectX::XMConvertToRadians(45.0f),0.0f };
       
        baseName = "FirstBase";
     //  pRenderer->Load(PATH_MODEL("base.obj"));
        break;
    case BaseKind::Second:
       m_tParam.m_f3Pos = { 0.0f ,-7.0f,-74.0f };
       m_tParam.m_f3Size = { 6.0f,1.0f,6.0f };
       m_tParam.m_f3Rotate = { 0.0f,DirectX::XMConvertToRadians(45.0f),0.0f };
        
        baseName = "SecondBase";
  //     pRenderer->Load(PATH_MODEL("base.obj"));
        break;
    case BaseKind::Third:
       m_tParam.m_f3Pos = { -70.0f,-7.0f,-146.0f };
       m_tParam.m_f3Size = { 6.0f,1.0f,6.0f };
       m_tParam.m_f3Rotate = { 0.0f,DirectX::XMConvertToRadians(45.0f),0.0f };

        baseName = "ThirdBase";
 //       pRenderer->Load(PATH_MODEL("base.obj"));
        break;
    default:
        break;
    }
  //  pRenderer->LoadVertexShader(PATH_SHADER("VS_Object.cso"));
    //pRenderer->LoadPixelShader(PATH_SHADER("PS_TexColor.cso"));

    CCollisionBox* pCollision = GetComponent<CCollisionBox>();
    pCollision->SetTag(baseName);
    pCollision->SetInfo(m_tParam.m_f3Pos, m_tParam.m_f3Size);
}

void CBase::Update()
{
    GetComponent<CCollisionBox>()->SetInfo(m_tParam.m_f3Pos, m_tParam.m_f3Size);

    CGameObject::Update();
}

#include "Goal.h"
#include "Sprite.h"
#include "Defines.h"

CGoal::CGoal()
    : m_pGoalTex(nullptr), m_pCamera(nullptr)
{
    m_pGoalTex = new Texture();
    if (FAILED(m_pGoalTex->Create(TEXPASS("First.png")))) 
    {
        MessageBox(NULL, "Texture load failed", "Goal.cpp", MB_OK);
    }
    m_pos = { -120.0f + WORLD_AJUST,0.0f + WORLD_AJUST,80.0f + WORLD_AJUST };
    m_size = { 10.0f,10.0f,10.0f };

}

CGoal::~CGoal()
{
    if (m_pGoalTex) {
        delete m_pGoalTex;
        m_pGoalTex = nullptr;
    }
}

void CGoal::Update()
{

}

void CGoal::Draw()
{
    static float s_nDeg = 0;
    float fMoveCos = cosf(DirectX::XMConvertToRadians(s_nDeg)) * 1.0f * 0.5f + 4.0f;
;

    DirectX::XMMATRIX mCamInv = DirectX::XMMatrixIdentity();

    if (m_pCamera) 
    {
        // カメラの行列からビルボード行列を計算
        Sprite::SetView(m_pCamera->GetViewMatrix());
        Sprite::SetProjection(m_pCamera->GetProjectionMatrix());
        DirectX::XMFLOAT4X4 view = m_pCamera->GetViewMatrix(false);
        mCamInv = DirectX::XMLoadFloat4x4(&view);
        mCamInv = DirectX::XMMatrixInverse(nullptr,mCamInv);
        DirectX::XMStoreFloat4x4(&view,mCamInv);
        view._41 = view._42 = view._43 = 0.0f;  // 移動部分の値を打ち消す
        mCamInv = DirectX::XMLoadFloat4x4(&view);
    }

    // ビルボード込みのワールド行列を計算
    DirectX::XMMATRIX mWorld = mCamInv * DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
    mWorld = DirectX::XMMatrixTranspose(mWorld);

    // スプライトに設定するワールド行列を用意  
    DirectX::XMFLOAT4X4 world;
    DirectX::XMStoreFloat4x4(&world,mWorld);

    Sprite::SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
    Sprite::SetOffset({ 0.0f, fMoveCos });
    Sprite::SetSize({6.0f,4.0f});
    Sprite::SetWorld(world);
    Sprite::SetTexture(m_pGoalTex);
    Sprite::Draw();

    s_nDeg += 2;
}

bool CGoal::IsHit(Collision::Info collision)
{
    Collision::Info goal;
    goal.type = Collision::eBox;
    goal.box = { m_pos,m_size };
    Collision::Result result = Collision::Hit(collision, goal);
    return result.isHit;
}

void CGoal::SetCamera(CCamera* camera)
{
    m_pCamera = camera;
}

void CGoal::SetPos(DirectX::XMFLOAT3 pos)
{
    m_pos = pos;
}
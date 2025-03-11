#include "CameraPlayer.h"
#include "Input.h"

constexpr float CAMMOVE_POW = DirectX::XMConvertToRadians(1.0f);
constexpr float CAMROTATE_DEF = DirectX::XMConvertToRadians(180.0f);

CCameraPlayer::CCameraPlayer()
	: m_pPlayer(nullptr)
    , m_fYaw(CAMROTATE_DEF), m_fPitch(0.0f), m_tForward{}
{

}

CCameraPlayer::~CCameraPlayer()
{

}

void CCameraPlayer::Update()
{
    m_pos = m_pPlayer->GetPos();


    if (IsKeyPress('W'))
    {
        m_fPitch -= CAMMOVE_POW;
    }
    if (IsKeyPress('S'))
    {
        m_fPitch += CAMMOVE_POW;
    }
    if (IsKeyPress('A'))
    {
        m_fYaw -= CAMMOVE_POW;
    }
    if (IsKeyPress('D'))
    {
        m_fYaw += CAMMOVE_POW;
    }


    // 方向ベクトルの更新
    DirectX::XMVECTOR forwardVec = DirectX::XMVectorSet(
        cosf(m_fPitch) * sinf(m_fYaw),
        sinf(m_fPitch),
        cosf(m_fPitch) * cosf(m_fYaw),
        0.0f
    );
    
    // プレイヤーの向いている方向の逆にカメラを配置
    DirectX::XMVECTOR playerPosVec = XMLoadFloat3(&m_pos);
    DirectX::XMVECTOR cameraPosVec = DirectX::XMVectorSubtract(playerPosVec, DirectX::XMVectorScale(forwardVec, 5.0f));

    // **カメラの視線方向（向いている方向）を取得**
    DirectX::XMFLOAT3 look;
    DirectX::XMStoreFloat3(&look, cameraPosVec);

    // **m_look に視線方向を代入**
    m_look.x = look.x;
    m_look.y = look.y;
    m_look.z = look.z;

}

DirectX::XMFLOAT3 CCameraPlayer::GetForward()
{
    return DirectX::XMFLOAT3(
        sinf(m_fYaw) * cosf(m_fPitch),
        sinf(m_fPitch),
        cosf(m_fYaw) * cosf(m_fPitch)
    );
}

void CCameraPlayer::SetPlayer(CPlayer* player)
{
	m_pPlayer = player;
}

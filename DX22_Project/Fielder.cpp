#include "Fielder.h"
#include "ModelRenderer.h"
#include "Main.h"
#include "Field.h"
#include "Ball.h"
#include "GameManager.h"
#include "Input.h"
#include "Oparation.h"

constexpr float ce_fDifencePower = 0.2f;	// 守備移動速度
constexpr float ce_fDifence = 0.4f;			// 守備操作速度
constexpr float ce_fThrowingPower = 2.0f;	// 送球の強さ

CFielder::CFielder()
    : CGameObject()
    , m_tFielderData(), m_ePosition(Positions::Pitcher)
    , m_bIsOparation(true), m_bChatch(false)
    , m_bMostNearToBase{}, m_pCollision{}
{
    m_tParam.m_f3Size = { 5.0f,5.0f,5.0f };
}

CFielder::~CFielder()
{

}

void CFielder::Init()
{
    CModelRenderer* pRenderer = AddComponent<CModelRenderer>();
    pRenderer->Load(PATH_MODEL("Ball.obj"));
    pRenderer->LoadVertexShader(PATH_SHADER("VS_Object.cso"));
    pRenderer->LoadPixelShader(PATH_SHADER("PS_TexColor.cso"));

    m_pCollision = AddComponent<CCollisionBox>();
    m_pCollision->SetTag("Fielder");
    m_pCollision->SetInfo(m_tParam.m_f3Pos, m_tParam.m_f3Size);

}

void CFielder::Update()
{
    CGameManager* pGameManager = CGameManager::GetInstance();
    GamePhase ePhase = pGameManager->GetPhase();

    if (ePhase == GamePhase::InPlay)
    {
        int DefencePlayer = pGameManager->GetDefenceManager()->GetPlayerNo();
        int a = 0;
        switch (m_ePosition)
        {
        case Positions::Pitcher:
            a = 0;
            break;
        case Positions::Chatcher:
            a = 0;
            break;
        case Positions::First:
            a = 0;
            break;
        case Positions::Second:
            a = 0;
            break;
        case Positions::Third:
            a = 0;
            break;
        case Positions::Short:
            a = 0;
            break;
        case Positions::Left:
            a = 0;
            break;
        case Positions::Center:
            a = 0;
            break;
        case Positions::Right:
            a = 0;
            break;
        case Positions::Max:
            break;
        case Positions::None:
            break;
        default:
            break;
        }

        if (m_bIsOparation)
        {
            CBall* pBall = GetScene()->GetGameObject<CBall>();
            // 移動処理	
            float fMovePow = int(m_tFielderData.m_eDefence) * (0.45f / 7.0f) + 0.2f;
            if (IsKeyPress(DefencePlayer, Input::Up))	m_tParam.m_f3Pos.z += fMovePow;
            if (IsKeyPress(DefencePlayer, Input::Down)) m_tParam.m_f3Pos.z -= fMovePow;
            if (IsKeyPress(DefencePlayer, Input::Left)) m_tParam.m_f3Pos.x -= fMovePow;
            if (IsKeyPress(DefencePlayer, Input::Right))m_tParam.m_f3Pos.x += fMovePow;

            if (m_bChatch)
            {
                pBall->SetPos(m_tParam.m_f3Pos);
                // 送球処理
                if (IsKeyPress(DefencePlayer, Input::B)) Throwing(BaseKind::First);
                if (IsKeyPress(DefencePlayer, Input::Y)) Throwing(BaseKind::Second);
                if (IsKeyPress(DefencePlayer, Input::X)) Throwing(BaseKind::Third);
                if (IsKeyPress(DefencePlayer, Input::A)) Throwing(BaseKind::Home);
            }
        }
            
    }

    // 当たり判定情報の更新
    m_pCollision->SetInfo(m_tParam.m_f3Pos, m_tParam.m_f3Size);

    CGameObject::Update();
}

void CFielder::LateUpdate()
{
    if (m_bIsOparation) return;
    BaseCover();
}

void CFielder::ResetPos()
{
    // 計算に使う変数の定義
    CField* pField = GetScene()->GetGameObject<CField>();
    DirectX::XMFLOAT3 fFieldPos = pField->GetPos();
    DirectX::XMFLOAT3 fFieldSizeMin = pField->GetSize();
    DirectX::XMFLOAT3 fFieldSize = { fFieldSizeMin.x * 8.0f , fFieldSizeMin.y , fFieldSizeMin.z * 8.0f };
    DirectX::XMFLOAT3 fFieldPosLine = { fFieldSize.x / 10.0f,fFieldSize.y ,fFieldSize.z / 10.0f };

    switch (m_ePosition)
    {
    case Positions::Pitcher:
        m_tParam.m_f3Pos = { fFieldPos.x,fFieldPos.y, fFieldPos.z - 105.0f };
        break;
    case Positions::Chatcher:
        m_tParam.m_f3Pos = { fFieldPos.x,fFieldPos.y, fFieldPos.z - 190.0f };
        break;
    case Positions::First:
        m_tParam.m_f3Pos = { fFieldPos.x + 70.0f,fFieldPos.y,  fFieldPos.z - 105.0f };
        break;
    case Positions::Second:
        m_tParam.m_f3Pos = { fFieldPos.x + 50.0f,fFieldPos.y, fFieldPos.z - 30.0f };
        break;
    case Positions::Third:
        m_tParam.m_f3Pos = { fFieldPos.x - 70.0f,fFieldPos.y,  fFieldPos.z - 105.0f };
        break;
    case Positions::Short:
        m_tParam.m_f3Pos = { fFieldPos.x - 50.0f,fFieldPos.y, fFieldPos.z - 30.0f };
        break;
    case Positions::Left:
        m_tParam.m_f3Pos = { fFieldPos.x - 120.0f,fFieldPos.y, fFieldPos.z + 80.0f };
        break;
    case Positions::Center:
        m_tParam.m_f3Pos = { fFieldPos.x,fFieldPos.y, fFieldPos.z + 140.0f };
        break;
    case Positions::Right:
        m_tParam.m_f3Pos = { fFieldPos.x + 120.0f,fFieldPos.y, fFieldPos.z + 80.0f };
        break;
    default:
        break;
    }
    m_tParam.m_f3Pos.y += m_tParam.m_f3Size.y / 2.0f;
    m_pCollision->SetInfo(m_tParam.m_f3Pos, m_tParam.m_f3Size);
}

void CFielder::OnCollision(CCollisionBase* other, std::string thisTag, Collision::Result result)
{
    CBall* pBall = dynamic_cast<CBall*>(other->GetGameObject());
    CBase* pBase = dynamic_cast<CBase*>(other->GetGameObject());

    if (pBall)
    {
        m_bChatch = true;
        m_bIsOparation = true;
        pBall->SetActive(false);
        bool isFry = pBall->GetIsFryBall();
        if (isFry)
        {
            CGameManager::GetInstance()->GetCountManager()->AddOutCount();
        }
        return;
    }
    if (pBase)
    {
        pBase->SetBaseCover(true);

        return;
    }
}

void CFielder::SetData(CFielderData* fielder)
{
    m_tFielderData = fielder->GetFielderData();
    m_ePosition = fielder->GetPlayerData().m_eEntryPosition;
}

void CFielder::SetData(FielderData data)
{
    m_tFielderData = data;
    m_ePosition = Positions::Pitcher;
}

bool CFielder::SetBaseCoverFrag(int baseIndex, bool frag)
{
    m_bMostNearToBase[baseIndex] = frag;

    return true;
}

void CFielder::BaseCover()
{
    std::list<CBase*> pBase = GetScene()->GetSameGameObject<CBase>();
    int index = 0;
    for (auto itr : pBase)
    {
        if (itr->IsBaseCover())
        {
            ++index;
            continue;
        }
        if (!m_bMostNearToBase[index])
        {
            ++index;
            continue;
        }
        DirectX::XMFLOAT3 fBasePos = itr->GetPos();
        
        DirectX::XMVECTOR vecBasePos = DirectX::XMLoadFloat3(&fBasePos);
        DirectX::XMVECTOR vecMemberPos = DirectX::XMLoadFloat3(&m_tParam.m_f3Pos);
        DirectX::XMVECTOR vecDirection = DirectX::XMVectorSubtract(vecBasePos, vecMemberPos);
        vecDirection = DirectX::XMVector3Normalize(vecDirection);
        vecDirection = DirectX::XMVectorScale(vecDirection, ce_fDifencePower);
        DirectX::XMFLOAT3 fMove;
        DirectX::XMStoreFloat3(&fMove, vecDirection);
        m_tParam.m_f3Pos.x += fMove.x;
        m_tParam.m_f3Pos.z += fMove.z;

        ++index;
    }
}

void CFielder::Throwing(BaseKind kind)
{
    std::list<CBase*> pField = GetScene()->GetSameGameObject<CBase>();
    CBase* pBase = nullptr;
    for (auto itr : pField)
    {
        if (itr->GetKind() == kind) pBase = itr;
    }
    if (!pBase) return;

    CScene* pScene = GetScene();
    CBall* pBall = pScene->GetGameObject<CBall>();
    CGameManager* pManager = CGameManager::GetInstance();
    CTeamDirector* pTeamManager = pManager->GetTeamManager(pManager->GetDefenceManager()->GetPlayerNo());

    DirectX::XMFLOAT3 fDir = pBase->GetPos() - m_tParam.m_f3Pos;

    DirectX::XMVECTOR vecDistance = DirectX::XMLoadFloat3(&fDir);
    DirectX::XMVECTOR vecThrowLength = DirectX::XMVector3Length(vecDistance);
    float fThrowAngle = DirectX::XMVectorGetX(vecThrowLength);
    fThrowAngle /= 400.0f;

    DirectX::XMVECTOR vecDirection = DirectX::XMVector3Normalize(vecDistance);
    DirectX::XMVECTOR vecBallPos = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_tParam.m_f3Pos), DirectX::XMVectorScale(vecDirection, m_tParam.m_f3Size.x));
    DirectX::XMFLOAT3 fNewBallPos;
    DirectX::XMStoreFloat3(&fNewBallPos, vecBallPos);
    pBall->SetPos(fNewBallPos);
    
    float fThrowPow = int(m_tFielderData.m_eThrowing) * (3.5f / 7.0f) + 0.5f;
    DirectX::XMVECTOR vecVelocity = DirectX::XMVectorScale(vecDirection, ce_fThrowingPower);
    DirectX::XMFLOAT3 fVelocity;
    DirectX::XMStoreFloat3(&fVelocity, vecVelocity);

    fVelocity.y = fThrowAngle;
    pBall->SetVelocity(fVelocity);
    m_bChatch = false;
}

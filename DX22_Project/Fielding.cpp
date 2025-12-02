#include "Fielding.h"
#include "Main.h"
#include "GameManager.h"
#include "Oparation.h"

// ==============================
//    メモ
// ==============================
// Difence0.4f...守備D 
// Difence0.2f...守備G
// 
// Throwing2.0f...肩力D
// Throwing0.5f...肩力G

CFielding::CFielding()
    : m_pFielder{}
{

}

CFielding::~CFielding()
{

}

void CFielding::Init()
{
    CScene* pScene = GetScene();
    
    for (int i = 0; i < m_pFielder.size(); i++)
    {
        m_pFielder[i] = pScene->AddGameObject<CFielder>("Fielder", Tag::GameObject);
    }

}

void CFielding::Update(int DefencePlayer)
{
    if (CGameManager::GetInstance()->GetPhase() != GamePhase::InPlay) return;
    m_pActiveFielderList.clear();
    for (int i = 0; i < m_pFielder.size(); i++)
    {
        m_pActiveFielderList.push_back(m_pFielder[i]);
        m_pActiveFielderList[i]->SetOparation(false);
        for (int j = 0; j < int(BaseKind::Max); j++)
        {
            m_pActiveFielderList[i]->SetBaseCoverFrag(j, false);
        }
    }

    CFielder* pMostNear = nullptr;
    DirectX::XMVECTOR mostNear = DirectX::XMVectorSet(FLT_MAX, FLT_MAX, FLT_MAX, 0.0f);
    CBall* pBall = GetScene()->GetGameObject<CBall>();
    int nMostNearIndex = 0;
    for (int i = 0; i < m_pActiveFielderList.size(); i++)
    {
        DirectX::XMFLOAT3 distance = pBall->GetPos() - m_pActiveFielderList[i]->GetPos();
        DirectX::XMVECTOR vecDistance = DirectX::XMVector3Length(DirectX::XMLoadFloat3(&distance));
        if (DirectX::XMVector3LessOrEqual(vecDistance, mostNear))
        {
            mostNear = vecDistance;
            pMostNear = m_pActiveFielderList[i];
            nMostNearIndex = i;
        }
    }
    pMostNear->SetOparation(true);
    m_pActiveFielderList.erase(m_pActiveFielderList.begin() + nMostNearIndex);
    
    auto baseList = GetScene()->GetSameGameObject<CBase>();
    nMostNearIndex = 0;
    int nBaseIndex = 0;
    for (auto itr : baseList)
    {
        mostNear = DirectX::XMVectorSet(FLT_MAX, FLT_MAX, FLT_MAX, 0.0f);
        for (int i = 0; i < m_pActiveFielderList.size(); i++)
        {

            DirectX::XMFLOAT3 distance = itr->GetPos() - m_pActiveFielderList[i]->GetPos();
            DirectX::XMVECTOR vecDistance = DirectX::XMVector3Length(DirectX::XMLoadFloat3(&distance));
            if (DirectX::XMVector3LessOrEqual(vecDistance, mostNear))
            {
                mostNear = vecDistance;
                pMostNear = m_pActiveFielderList[i];
                nMostNearIndex = i;
            }

        }
        pMostNear->SetBaseCoverFrag(nBaseIndex, true);
        nBaseIndex++;
        m_pActiveFielderList.erase(m_pActiveFielderList.begin() + nMostNearIndex);
    }
}

void CFielding::SetFielderData(int DefencePlayer)
{
    CTeamDirector* pTeamDirector = CGameManager::GetInstance()->GetTeamManager(DefencePlayer);

    for (auto itr : pTeamDirector->GetTeam()->GetStartingLineup())
    {
        PlayerData data = itr->GetPlayerData();

        if (data.m_eEntryPosition != Positions::Max)
        {
            m_pFielder[int(data.m_eEntryPosition)]->SetData(itr);
            m_pFielder[int(data.m_eEntryPosition)]->ResetPos();
        }
    }
    FielderData data{};
    data.m_eDefence = Quality::D;
    data.m_eThrowing = Quality::D;
    data.m_eChatch = Quality::D;
    m_pFielder[int(Positions::Pitcher)]->SetData(data);
    m_pFielder[int(Positions::Pitcher)]->ResetPos();
}

//void CFielding::OutProcess(CRunning::RunnerParam* RunnerParam, CField::BaseKind kind)
//{
//	CField* pField = CField::GetInstance().get();
//
//	DirectX::XMFLOAT3 fBasePos = pField->GetBasePos(kind);
//	DirectX::XMFLOAT3 fBaseSize = pField->GetBaseSize(kind);
//	Collision::Info2D op;	// 操作選手のxz当たり判定
//	Collision::Info2D base;	// ベースのxz当たり判定
//	op.type = Collision::eSquare;
//	op.square.pos = { m_tParam[m_nOperationNo].pos.x,m_tParam[m_nOperationNo].pos.z };
//	op.square.size = { m_tParam[m_nOperationNo].size.x,m_tParam[m_nOperationNo].size.z };
//	base.type = Collision::eSquare;
//	base.square.pos = { fBasePos.x,fBasePos.z };
//	base.square.size = { fBaseSize.x,fBaseSize.z };
//
//	// ベースを踏んだ時
//	if (Collision::Hit2D(op, base).isHit)
//	{
//		switch (kind)
//		{
//			// 一塁ベースを踏む事で取るアウト
//		case CField::BaseKind::First:
//			// ランナー飛び出し時の処理
//			if (m_eChatch == ChatchPattern::Fry && // 打球がフライかつ
//				RunnerParam[(int)CRunning::RunnerKind::FirstRunner].m_bAlive &&	// 一塁ランナーが存在している状態かつ
//				!RunnerParam[(int)CRunning::RunnerKind::FirstRunner].m_bStayPrevBase)	// ランナーが元の塁から離れている状態
//			{
//				CRunning::SetOut(CRunning::RunnerKind::FirstRunner);	// 一塁ランナーをアウトにする
//			}
//			// 送球アウトの処理
//			if (m_eChatch == ChatchPattern::Grounder &&	// 打球がゴロかつ
//				RunnerParam[(int)CRunning::RunnerKind::BatterRunner].m_bAlive &&	// バッターランナーが存在している状態かつ
//				RunnerParam[(int)CRunning::RunnerKind::BatterRunner].m_eArriveKind != CField::BaseKind::First)	// バッターランナーが一塁に到達してない時
//			{
//				CRunning::SetOut(CRunning::RunnerKind::BatterRunner);	// バッターランナーをアウトにする
//			}
//			break;
//		case CField::BaseKind::Second:
//			// ランナー飛び出し時の処理
//			if (m_eChatch == ChatchPattern::Fry && // 打球がフライかつ
//				RunnerParam[(int)CRunning::RunnerKind::SecondRunner].m_bAlive &&	// 二塁ランナーが存在している状態かつ
//				!RunnerParam[(int)CRunning::RunnerKind::SecondRunner].m_bStayPrevBase)	// ランナーが元の塁から離れている状態
//			{
//				CRunning::SetOut(CRunning::RunnerKind::SecondRunner);	// 二塁ランナーをアウトにする
//			}
//			// 送球アウトの処理
//			if (m_eChatch == ChatchPattern::Grounder &&	// 打球がゴロかつ
//				RunnerParam[(int)CRunning::RunnerKind::FirstRunner].m_bAlive &&	// 一塁ランナーが存在している状態かつ
//				RunnerParam[(int)CRunning::RunnerKind::FirstRunner].m_eArriveKind != CField::BaseKind::Second &&	// 一塁ランナーが二塁に到達してない時かつ
//				RunnerParam[(int)CRunning::RunnerKind::BatterRunner].m_bAlive)	// 手前にランナーが詰まっている時
//			{
//				CRunning::SetOut(CRunning::RunnerKind::FirstRunner);	// 一塁ランナーをアウトにする
//			}
//			break;
//		case CField::BaseKind::Third:
//			// ランナー飛び出し時の処理
//			if (m_eChatch == ChatchPattern::Fry && // 打球がフライかつ
//				RunnerParam[(int)CRunning::RunnerKind::ThirdRunner].m_bAlive &&	// 三塁ランナーが存在している状態かつ
//				!RunnerParam[(int)CRunning::RunnerKind::ThirdRunner].m_bStayPrevBase)	// ランナーが元の塁から離れている状態
//			{
//				CRunning::SetOut(CRunning::RunnerKind::ThirdRunner);	// 三塁ランナーをアウトにする
//			}
//			// 送球アウトの処理
//			if (m_eChatch == ChatchPattern::Grounder &&	// 打球がゴロかつ
//				RunnerParam[(int)CRunning::RunnerKind::SecondRunner].m_bAlive &&	// 二塁ランナーが存在している状態かつ
//				RunnerParam[(int)CRunning::RunnerKind::SecondRunner].m_eArriveKind != CField::BaseKind::Third &&	// 二塁ランナーが三塁に到達してない時かつ
//				RunnerParam[(int)CRunning::RunnerKind::FirstRunner].m_bAlive)	// 手前にランナーが詰まっている時
//			{
//				CRunning::SetOut(CRunning::RunnerKind::SecondRunner);	// 二塁ランナーをアウトにする
//			}
//			break;
//		case CField::BaseKind::Home:
//			if (m_eChatch == ChatchPattern::Grounder &&	// 打球がゴロかつ
//				RunnerParam[(int)CRunning::RunnerKind::ThirdRunner].m_bAlive &&	// 三塁ランナーが存在している状態かつ
//				RunnerParam[(int)CRunning::RunnerKind::ThirdRunner].m_eArriveKind != CField::BaseKind::Home &&	// 三塁ランナーが本塁に到達してない時かつ
//				RunnerParam[(int)CRunning::RunnerKind::SecondRunner].m_bAlive)	// 手前にランナーが詰まっている時
//			{
//				CRunning::SetOut(CRunning::RunnerKind::ThirdRunner);	// 三塁ランナーをアウトにする
//			}
//			break;
//		case CField::BaseKind::Max:
//			break;
//		default:
//			break;
//		}
//	}
//
//}

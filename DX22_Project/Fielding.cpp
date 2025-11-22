#include "Fielding.h"

// ==============================
//    メモ
// ==============================
// Difence0.4f...守備D 
// Difence0.2f...守備G
// 
// Throwing2.0f...肩力D
// Throwing0.5f...肩力G

CFielding::CFielding()
{

}

CFielding::~CFielding()
{
}

void CFielding::Init()
{
	
}

void CFielding::Update(int DefencePlayer)
{

}

//
//int CFielding::OperationSearch()
//{
//	DirectX::XMFLOAT3 fBallPos = CBall::GetInstance()->GetPos();
//	DirectX::XMVECTOR vecBallPos = DirectX::XMLoadFloat3(&fBallPos);
//	DirectX::XMVECTOR vecMostNear = DirectX::XMVectorSet(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
//	int nMostNearNo = -1;
//
//	for (int i = 0; i < (int)FieldMember::Max; i++)
//	{
//		DirectX::XMVECTOR vecMemberPos = DirectX::XMLoadFloat3(&m_tParam[i].pos);
//		DirectX::XMVECTOR vecDistance = DirectX::XMVectorSubtract(vecBallPos, vecMemberPos);
//		vecDistance = DirectX::XMVector3Length(vecDistance);
//
//		if (DirectX::XMVector3Less(vecDistance, vecMostNear) || i == 0)
//		{
//			vecMostNear = vecDistance;
//			nMostNearNo = i;
//		}
//	}
//	return nMostNearNo;
//}
//
//int CFielding::BaseSearch(CField::BaseKind kind)
//{
//	DirectX::XMFLOAT3 fBasePos = CField ::GetInstance()->GetBasePos(kind);
//	DirectX::XMVECTOR vecBasePos = DirectX::XMLoadFloat3(&fBasePos);
//	DirectX::XMVECTOR vecMostNear = DirectX::XMVectorSet(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
//	int nMostNearNo = -1;
//
//	for (int i = 0; i < (int)FieldMember::Max; i++)
//	{
//		DirectX::XMVECTOR vecMemberPos = DirectX::XMLoadFloat3(&m_tParam[i].pos);
//		DirectX::XMVECTOR vecDistance = DirectX::XMVectorSubtract(vecBasePos, vecMemberPos);
//		vecDistance = DirectX::XMVector3Length(vecDistance);
//
//		if (i == m_nOperationNo)continue;
//		for (int j = 0; j < (int)CField::BaseKind::Max; j++)
//		{
//			if (i == m_nBaseNearNo[j])continue;
//		}
//
//		if (DirectX::XMVector3Less(vecDistance, vecMostNear))
//		{
//			vecMostNear = vecDistance;
//			nMostNearNo = i;
//		}
//	}
//	return nMostNearNo;
//}

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

// ==============================
//    インクルード部
// ==============================
#include "Fielding.h"
#include "Camera.h"
#include "Main.h"
#include "Ball.h"
#include "BallCount.h"
#include "Input.h"
#include "Running.h"
#include "TeamManager.h"
#include "GameManager.h"

// ==============================
//    定数定義
// ==============================


// ==============================
//    静的変数の初期化
// ==============================
CFielding::ChatchPattern CFielding::m_eChatch = ChatchPattern::NotChatch;

// ==============================
//    メモ
// ==============================
// Difence0.4f...守備D 
// Difence0.2f...守備G
// 
// Throwing2.0f...肩力D
// Throwing0.5f...肩力G

CFielding::CFielding()
	: m_bHold(false), m_nOperationNo(0), m_nBaseNearNo{ -1,-1,-1,-1 }
	, m_bBaseCovered{},m_fThrowDirection{}
{
	// モデルパラメータの初期化
	for (int i = 0; i < (int)FieldMember::Max; i++)
	{
		m_pFieldMember[i] = std::make_unique<Model>();
		m_tParam[i].pos = { WORLD_AJUST,WORLD_AJUST, WORLD_AJUST };
		m_tParam[i].size = { 5.0f,5.0f,5.0f };
		m_tParam[i].rotate = { 0.0f,0.0f,0.0f };
		m_Collision[i].type = Collision::eBox;
		m_Collision[i].box.center = m_tParam[i].pos;
		m_Collision[i].box.size = m_tParam[i].size;
	}

}

CFielding::~CFielding()
{
}

void CFielding::Update(int DefencePlayer)
{
	CScene* pScene = GetScene();
	CField* pField = pScene->GetGameObject<CField>();
	CBall* pBall = pScene->GetGameObject<CBall>();
	CGameManager* pGameManager = CGameManager::GetInstance();
	//CTeamManager* pTeamManager = pGameManager->GetTeamManager(DefencePlayer);

	// 計算に使う変数の定義
	DirectX::XMFLOAT3 fFieldPos = pField->GetPos();
	fFieldPos.y = WORLD_AJUST + ce_fGroundY;
	DirectX::XMFLOAT3 fFieldSizeMin = pField->GetSize();
	DirectX::XMFLOAT3 fFieldSize = { fFieldSizeMin.x * 8.0f , fFieldSizeMin.y , fFieldSizeMin.z * 8.0f };
	DirectX::XMFLOAT3 fFieldPosLine = {fFieldSize.x / 10.0f,fFieldSize.y ,fFieldSize.z / 10.0f };

	switch (pGameManager->GetPhase())
	{
	case GamePhase::Batting:

				break;
	case GamePhase::InPlay:


			if (!m_bHold)
			{
				Collision::Info ballCollision = pBall->GetCollision();

				// ボールとの当たり判定チェック(捕球処理)
				for (int i = 0; i < (int)FieldMember::Max; i++)
				{
					Collision::Result result = Collision::Hit(ballCollision, m_Collision[i]);
					if (result.isHit)
					{
						// 操作選手をボールを持っている選手にする
						m_nOperationNo = i;
						m_bHold = true;

						// ボールが転がっている状態ではなくなるので、インプレー終了要素を満たす
						pGameManager->SetEndInplay(CGameManager::InplayElement::HoldBall, true);

						// フライを捕球したらアウトにする
						if (pBall->GetIsFry())
						{
							// フライアウトの処理
							CRunning::SetOut(CRunning::RunnerKind::BatterRunner);
							m_eChatch = ChatchPattern::Fry;
						}
						//フライでない場合はゴロ捕球にする
						if(m_eChatch != ChatchPattern::Fry)m_eChatch = ChatchPattern::Grounder;
					}
				}
			}
			else
			{
				// 送球処理
				if (IsKeyPress(DefencePlayer, Input::B))	Throwing(CField::BaseKind::First);
				if (IsKeyPress(DefencePlayer, Input::Y)) Throwing(CField::BaseKind::Second);
				if (IsKeyPress(DefencePlayer, Input::X)) Throwing(CField::BaseKind::Third);
				if (IsKeyPress(DefencePlayer, Input::A))Throwing(CField::BaseKind::Home);

				CRunning::RunnerParam tParam[(int)CRunning::RunnerKind::Max];
				// 各ランナーの状態を取得
				for (int i = 0; i < (int)CRunning::RunnerKind::Max; i++)
				{
					tParam[i] = CRunning::GetRannerParam((CRunning::RunnerKind)i);
				}

				// フォースプレイの処理
				for (int i = 0; i < (int)CField::BaseKind::Max; i++)
				{
					OutProcess(tParam, (CField::BaseKind)i);
				}

				// タッチアウトの処理
				for (int i = 0; i < (int)CRunning::RunnerKind::Max; i++)
				{
					if (Collision::Hit(m_Collision[m_nOperationNo],CRunning::GetCollision((CRunning::RunnerKind)i)).isHit)
					{
						if(!CRunning::GetOnBase((CRunning::RunnerKind)i))CRunning::SetOut((CRunning::RunnerKind)i);
					}
				}
				
			}
		}
		break;
	default:
		break;
	}
}

void CFielding::Draw()
{

}

void CFielding::BaseCover()
{
	CField* pField = CField::GetInstance().get();
	for (int i = 0; i < (int)CField::BaseKind::Max; i++)
	{
		m_bBaseCovered[i] = false;
		DirectX::XMFLOAT3 fBasePos = pField->GetBasePos((CField::BaseKind)i);
		DirectX::XMFLOAT3 fBaseSize = pField->GetBaseSize((CField::BaseKind)i);
		Collision::Info2D member;
		Collision::Info2D base;
		member.type = Collision::eSquare;
		member.square.pos = { m_tParam[m_nBaseNearNo[i]].pos.x,m_tParam[m_nBaseNearNo[i]].pos.z };
		member.square.size = { m_tParam[m_nBaseNearNo[i]].size.x,m_tParam[m_nBaseNearNo[i]].size.z };
		base.type = Collision::eSquare;
		base.square.pos = { fBasePos.x,fBasePos.z };
		base.square.size = { fBaseSize.x,fBaseSize.z };
		if (Collision::Hit2D(member, base).isHit)
		{
			m_bBaseCovered[i] = true;
			continue;
		}
		DirectX::XMVECTOR vecBasePos = DirectX::XMLoadFloat3(&fBasePos);
		DirectX::XMVECTOR vecMemberPos = DirectX::XMLoadFloat3(&m_tParam[m_nBaseNearNo[i]].pos);
		DirectX::XMVECTOR vecDirection = DirectX::XMVectorSubtract(vecBasePos, vecMemberPos);
		vecDirection = DirectX::XMVector3Normalize(vecDirection);
		vecDirection = DirectX::XMVectorScale(vecDirection, ce_fDifencePower);
		DirectX::XMFLOAT3 fMove;
		DirectX::XMStoreFloat3(&fMove, vecDirection);
		m_tParam[m_nBaseNearNo[i]].pos.x += fMove.x;
		m_tParam[m_nBaseNearNo[i]].pos.z += fMove.z;
	}
}

void CFielding::Throwing(CField::BaseKind kind)
{
	if(!m_bBaseCovered[(int)kind]) return;

	CBall* pBall = CBall::GetInstance().get();
	CTeamManager* pTeamManager = CTeamManager::GetInstance((int)CGameManager::GetInstance()->GetDefenseTeam()).get();
	
	DirectX::XMFLOAT3 fBaseCoverPos = m_tParam[m_nBaseNearNo[(int)kind]].pos;
	DirectX::XMFLOAT3 fBallPos = pBall->GetPos();
	DirectX::XMVECTOR vecBallPos = DirectX::XMLoadFloat3(&fBallPos);
	DirectX::XMVECTOR vecBaseCoverPos = DirectX::XMLoadFloat3(&fBaseCoverPos);
	DirectX::XMVECTOR vecDirection = DirectX::XMVectorSubtract(vecBaseCoverPos, vecBallPos);
	DirectX::XMVECTOR vecThrowLength =  DirectX::XMVector3Length(vecDirection);
	float fThrowAngle = DirectX::XMVectorGetX(vecThrowLength);
	fThrowAngle /= 400.0f;

	vecDirection = DirectX::XMVector3Normalize(vecDirection);
	vecBallPos = DirectX::XMVectorAdd(vecBallPos, DirectX::XMVectorScale(vecDirection, 5.0f));
	DirectX::XMFLOAT3 fNewBallPos;
	DirectX::XMStoreFloat3(&fNewBallPos, vecBallPos);
	pBall->SetPos(fNewBallPos);
	float fThrowPow = pTeamManager->GetFielderState((CTeamManager::FieldingNo)(m_nOperationNo + 1)).m_eThrowing * (3.5f / 7.0f) + 0.5f;
	vecDirection = DirectX::XMVectorScale(vecDirection, ce_fThrowingPower);
	DirectX::XMStoreFloat3(&m_fThrowDirection, vecDirection);

	m_fThrowDirection.y = fThrowAngle;
	pBall->SetMove(m_fThrowDirection);
	m_bHold = false;
}

int CFielding::OperationSearch()
{
	DirectX::XMFLOAT3 fBallPos = CBall::GetInstance()->GetPos();
	DirectX::XMVECTOR vecBallPos = DirectX::XMLoadFloat3(&fBallPos);
	DirectX::XMVECTOR vecMostNear = DirectX::XMVectorSet(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
	int nMostNearNo = -1;

	for (int i = 0; i < (int)FieldMember::Max; i++)
	{
		DirectX::XMVECTOR vecMemberPos = DirectX::XMLoadFloat3(&m_tParam[i].pos);
		DirectX::XMVECTOR vecDistance = DirectX::XMVectorSubtract(vecBallPos, vecMemberPos);
		vecDistance = DirectX::XMVector3Length(vecDistance);

		if (DirectX::XMVector3Less(vecDistance, vecMostNear) || i == 0)
		{
			vecMostNear = vecDistance;
			nMostNearNo = i;
		}
	}
	return nMostNearNo;
}

int CFielding::BaseSearch(CField::BaseKind kind)
{
	DirectX::XMFLOAT3 fBasePos = CField ::GetInstance()->GetBasePos(kind);
	DirectX::XMVECTOR vecBasePos = DirectX::XMLoadFloat3(&fBasePos);
	DirectX::XMVECTOR vecMostNear = DirectX::XMVectorSet(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
	int nMostNearNo = -1;

	for (int i = 0; i < (int)FieldMember::Max; i++)
	{
		DirectX::XMVECTOR vecMemberPos = DirectX::XMLoadFloat3(&m_tParam[i].pos);
		DirectX::XMVECTOR vecDistance = DirectX::XMVectorSubtract(vecBasePos, vecMemberPos);
		vecDistance = DirectX::XMVector3Length(vecDistance);

		if (i == m_nOperationNo)continue;
		for (int j = 0; j < (int)CField::BaseKind::Max; j++)
		{
			if (i == m_nBaseNearNo[j])continue;
		}

		if (DirectX::XMVector3Less(vecDistance, vecMostNear))
		{
			vecMostNear = vecDistance;
			nMostNearNo = i;
		}
	}
	return nMostNearNo;
}

void CFielding::OutProcess(CRunning::RunnerParam* RunnerParam, CField::BaseKind kind)
{
	CField* pField = CField::GetInstance().get();

	DirectX::XMFLOAT3 fBasePos = pField->GetBasePos(kind);
	DirectX::XMFLOAT3 fBaseSize = pField->GetBaseSize(kind);
	Collision::Info2D op;	// 操作選手のxz当たり判定
	Collision::Info2D base;	// ベースのxz当たり判定
	op.type = Collision::eSquare;
	op.square.pos = { m_tParam[m_nOperationNo].pos.x,m_tParam[m_nOperationNo].pos.z };
	op.square.size = { m_tParam[m_nOperationNo].size.x,m_tParam[m_nOperationNo].size.z };
	base.type = Collision::eSquare;
	base.square.pos = { fBasePos.x,fBasePos.z };
	base.square.size = { fBaseSize.x,fBaseSize.z };

	// ベースを踏んだ時
	if (Collision::Hit2D(op, base).isHit)
	{
		switch (kind)
		{
			// 一塁ベースを踏む事で取るアウト
		case CField::BaseKind::First:
			// ランナー飛び出し時の処理
			if (m_eChatch == ChatchPattern::Fry && // 打球がフライかつ
				RunnerParam[(int)CRunning::RunnerKind::FirstRunner].m_bAlive &&	// 一塁ランナーが存在している状態かつ
				!RunnerParam[(int)CRunning::RunnerKind::FirstRunner].m_bStayPrevBase)	// ランナーが元の塁から離れている状態
			{
				CRunning::SetOut(CRunning::RunnerKind::FirstRunner);	// 一塁ランナーをアウトにする
			}
			// 送球アウトの処理
			if (m_eChatch == ChatchPattern::Grounder &&	// 打球がゴロかつ
				RunnerParam[(int)CRunning::RunnerKind::BatterRunner].m_bAlive &&	// バッターランナーが存在している状態かつ
				RunnerParam[(int)CRunning::RunnerKind::BatterRunner].m_eArriveKind != CField::BaseKind::First)	// バッターランナーが一塁に到達してない時
			{
				CRunning::SetOut(CRunning::RunnerKind::BatterRunner);	// バッターランナーをアウトにする
			}
			break;
		case CField::BaseKind::Second:
			// ランナー飛び出し時の処理
			if (m_eChatch == ChatchPattern::Fry && // 打球がフライかつ
				RunnerParam[(int)CRunning::RunnerKind::SecondRunner].m_bAlive &&	// 二塁ランナーが存在している状態かつ
				!RunnerParam[(int)CRunning::RunnerKind::SecondRunner].m_bStayPrevBase)	// ランナーが元の塁から離れている状態
			{
				CRunning::SetOut(CRunning::RunnerKind::SecondRunner);	// 二塁ランナーをアウトにする
			}
			// 送球アウトの処理
			if (m_eChatch == ChatchPattern::Grounder &&	// 打球がゴロかつ
				RunnerParam[(int)CRunning::RunnerKind::FirstRunner].m_bAlive &&	// 一塁ランナーが存在している状態かつ
				RunnerParam[(int)CRunning::RunnerKind::FirstRunner].m_eArriveKind != CField::BaseKind::Second &&	// 一塁ランナーが二塁に到達してない時かつ
				RunnerParam[(int)CRunning::RunnerKind::BatterRunner].m_bAlive)	// 手前にランナーが詰まっている時
			{
				CRunning::SetOut(CRunning::RunnerKind::FirstRunner);	// 一塁ランナーをアウトにする
			}
			break;
		case CField::BaseKind::Third:
			// ランナー飛び出し時の処理
			if (m_eChatch == ChatchPattern::Fry && // 打球がフライかつ
				RunnerParam[(int)CRunning::RunnerKind::ThirdRunner].m_bAlive &&	// 三塁ランナーが存在している状態かつ
				!RunnerParam[(int)CRunning::RunnerKind::ThirdRunner].m_bStayPrevBase)	// ランナーが元の塁から離れている状態
			{
				CRunning::SetOut(CRunning::RunnerKind::ThirdRunner);	// 三塁ランナーをアウトにする
			}
			// 送球アウトの処理
			if (m_eChatch == ChatchPattern::Grounder &&	// 打球がゴロかつ
				RunnerParam[(int)CRunning::RunnerKind::SecondRunner].m_bAlive &&	// 二塁ランナーが存在している状態かつ
				RunnerParam[(int)CRunning::RunnerKind::SecondRunner].m_eArriveKind != CField::BaseKind::Third &&	// 二塁ランナーが三塁に到達してない時かつ
				RunnerParam[(int)CRunning::RunnerKind::FirstRunner].m_bAlive)	// 手前にランナーが詰まっている時
			{
				CRunning::SetOut(CRunning::RunnerKind::SecondRunner);	// 二塁ランナーをアウトにする
			}
			break;
		case CField::BaseKind::Home:
			if (m_eChatch == ChatchPattern::Grounder &&	// 打球がゴロかつ
				RunnerParam[(int)CRunning::RunnerKind::ThirdRunner].m_bAlive &&	// 三塁ランナーが存在している状態かつ
				RunnerParam[(int)CRunning::RunnerKind::ThirdRunner].m_eArriveKind != CField::BaseKind::Home &&	// 三塁ランナーが本塁に到達してない時かつ
				RunnerParam[(int)CRunning::RunnerKind::SecondRunner].m_bAlive)	// 手前にランナーが詰まっている時
			{
				CRunning::SetOut(CRunning::RunnerKind::ThirdRunner);	// 三塁ランナーをアウトにする
			}
			break;
		case CField::BaseKind::Max:
			break;
		default:
			break;
		}
	}

}

Collision::Info CFielding::GetCollision(FieldMember Member)
{
	return m_Collision[(int)Member];
}

CFielding::ChatchPattern CFielding::GetChatchPattern()
{
	return m_eChatch;
}

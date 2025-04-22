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

// ==============================
//    定数定義
// ==============================
constexpr float ce_fDifencePower = 0.2f;	// 守備移動速度
constexpr float ce_fDifence = 0.4f;			// 守備操作速度
constexpr float ce_fThrowingPower = 2.0f;	// 送球の強さ

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

	// モデルの読み込み
	if (!m_pFieldMember[(int)FieldMember::Pitcher]->Load(PATH_MODEL("ball.obj"))) ERROR_MESSAGE("");
	if (!m_pFieldMember[(int)FieldMember::Chatcher]->Load(PATH_MODEL("ball.obj")))ERROR_MESSAGE("");
	if (!m_pFieldMember[(int)FieldMember::First]->Load(PATH_MODEL("ball.obj")))ERROR_MESSAGE("");
	if (!m_pFieldMember[(int)FieldMember::Second]->Load(PATH_MODEL("ball.obj")))ERROR_MESSAGE("");
	if (!m_pFieldMember[(int)FieldMember::Third]->Load(PATH_MODEL("ball.obj")))ERROR_MESSAGE("");
	if (!m_pFieldMember[(int)FieldMember::Short]->Load(PATH_MODEL("ball.obj")))ERROR_MESSAGE("");
	if (!m_pFieldMember[(int)FieldMember::Left]->Load(PATH_MODEL("ball.obj")))ERROR_MESSAGE("");
	if (!m_pFieldMember[(int)FieldMember::Center]->Load(PATH_MODEL("ball.obj")))ERROR_MESSAGE("");
	if (!m_pFieldMember[(int)FieldMember::Right]->Load(PATH_MODEL("ball.obj")))ERROR_MESSAGE("");
}

CFielding::~CFielding()
{
}

void CFielding::Update()
{
	CField* pField = CField::GetInstance().get();
	CBall* pBall = CBall::GetInstance().get();
	CBallCount* pBallCount = CBallCount::GetInstance().get();

	// 計算に使う変数の定義
	DirectX::XMFLOAT3 fFieldPos = pField->GetPos();
	fFieldPos.y = WORLD_AJUST + ce_fGroundY;
	DirectX::XMFLOAT3 fFieldSizeMin = pField->GetSize();
	DirectX::XMFLOAT3 fFieldSize = { fFieldSizeMin.x * 8.0f , fFieldSizeMin.y , fFieldSizeMin.z * 8.0f };
	DirectX::XMFLOAT3 fFieldPosLine = {fFieldSize.x / 10.0f,fFieldSize.y ,fFieldSize.z / 10.0f };

	switch (CBall::GetInstance()->GetPhase())
	{
	case CBall::BallPhase::Batting:
		// 守備位置の初期化
		m_tParam[(int)FieldMember::Pitcher].pos = { fFieldPos.x,fFieldPos.y, fFieldPos.z + fFieldPosLine.z * 2.5f };
		m_tParam[(int)FieldMember::Chatcher].pos = { fFieldPos.x,fFieldPos.y, fFieldPos.z + fFieldPosLine.z * 7.3f };
		m_tParam[(int)FieldMember::First].pos = { fFieldPos.x - fFieldPosLine.x * 1.7f,fFieldPos.y, fFieldPos.z + fFieldPosLine.z * 2.5f };
		m_tParam[(int)FieldMember::Second].pos = { fFieldPos.x - fFieldPosLine.x * 0.9f,fFieldPos.y, fFieldPos.z + fFieldPosLine.z * 1.2f };
		m_tParam[(int)FieldMember::Third].pos = { fFieldPos.x + fFieldPosLine.x * 1.7f,fFieldPos.y, fFieldPos.z + fFieldPosLine.z * 2.5f };
		m_tParam[(int)FieldMember::Short].pos = { fFieldPos.x + fFieldPosLine.x * 0.9f,fFieldPos.y, fFieldPos.z + fFieldPosLine.z * 1.2f };
		m_tParam[(int)FieldMember::Left].pos = { fFieldPos.x + fFieldPosLine.x * 2.3f,fFieldPos.y, fFieldPos.z - fFieldPosLine.z * 1.7f };
		m_tParam[(int)FieldMember::Center].pos = { fFieldPos.x,fFieldPos.y, fFieldPos.z - fFieldPosLine.z * 2.3f };
		m_tParam[(int)FieldMember::Right].pos = { fFieldPos.x - fFieldPosLine.x * 2.3f,fFieldPos.y, fFieldPos.z - fFieldPosLine.z * 1.7f };
		
		m_bHold = false;
		m_eChatch = ChatchPattern::NotChatch;
		break;
	case CBall::BallPhase::InPlay:
		// インプレー終了時に抜け出さない用
		if (!pBallCount->GetEndInplay())
		{
			// ボールを持っていない時は操作選手の探索を行う
			if (!m_bHold)
			{
				m_nOperationNo = OperationSearch();
			}
			else
			{
				// ボールを持っている時はボールの位置を操作選手の位置にする
				pBall->SetPos(m_tParam[m_nOperationNo].pos);
				m_fThrowDirection = {};
			}

			// 移動処理
			if (pBallCount->GetDefenseTeam() == CBallCount::Team::Player1 ? IsKeyPress(InputPlayer1::Up) : IsKeyPress(InputPlayer2::Up)) m_tParam[m_nOperationNo].pos.z -= ce_fDifence;
			if (pBallCount->GetDefenseTeam() == CBallCount::Team::Player1 ? IsKeyPress(InputPlayer1::Down) : IsKeyPress(InputPlayer2::Down)) m_tParam[m_nOperationNo].pos.z += ce_fDifence;
			if (pBallCount->GetDefenseTeam() == CBallCount::Team::Player1 ? IsKeyPress(InputPlayer1::Left) : IsKeyPress(InputPlayer2::Left)) m_tParam[m_nOperationNo].pos.x -= ce_fDifence;
			if (pBallCount->GetDefenseTeam() == CBallCount::Team::Player1 ? IsKeyPress(InputPlayer1::Right) : IsKeyPress(InputPlayer2::Right)) m_tParam[m_nOperationNo].pos.x += ce_fDifence;

			// ベースに近い選手を初期化
			for (int i = 0; i < (int)CField::BaseKind::Max; i++)
			{
				m_nBaseNearNo[i] = -1;
			}
			// ベースに近い選手を操作選手以外から探索
			m_nBaseNearNo[(int)CField::BaseKind::First]	= BaseSearch(CField::BaseKind::First);
			m_nBaseNearNo[(int)CField::BaseKind::Second]= BaseSearch(CField::BaseKind::Second);
			m_nBaseNearNo[(int)CField::BaseKind::Third] = BaseSearch(CField::BaseKind::Third);
			m_nBaseNearNo[(int)CField::BaseKind::Home]	= BaseSearch(CField::BaseKind::Home);
			// ベースカバー処理
			BaseCover();

			// 当たり判定情報の更新
			for (int i = 0; i < (int)FieldMember::Max; i++)
			{
				m_Collision[i].type = Collision::eBox;
				m_Collision[i].box.center = m_tParam[i].pos;
				m_Collision[i].box.size = m_tParam[i].size;
			}

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
						pBallCount->SetEndInplay(CBallCount::InplayElement::HoldBall, true);

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
				if (pBallCount->GetDefenseTeam() == CBallCount::Team::Player1 ? IsKeyPress(InputPlayer1::B) : IsKeyPress(InputPlayer2::B))Throwing(CField::BaseKind::First);
				if (pBallCount->GetDefenseTeam() == CBallCount::Team::Player1 ? IsKeyPress(InputPlayer1::Y) : IsKeyPress(InputPlayer2::Y))Throwing(CField::BaseKind::Second);
				if (pBallCount->GetDefenseTeam() == CBallCount::Team::Player1 ? IsKeyPress(InputPlayer1::X) : IsKeyPress(InputPlayer2::X))Throwing(CField::BaseKind::Third);
				if (pBallCount->GetDefenseTeam() == CBallCount::Team::Player1 ? IsKeyPress(InputPlayer1::A) : IsKeyPress(InputPlayer2::A))Throwing(CField::BaseKind::Home);

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
	for (int i = 0; i < (int)FieldMember::Max; i++)
	{

		if (CBall::GetInstance()->GetPhase() == CBall::BallPhase::Batting && i == (int)FieldMember::Chatcher)continue;

		SetModel(m_tParam[i],m_pFieldMember[i].get());
		Collision::DrawCollision(m_Collision[i]);
	}
}

void CFielding::SetModel(ModelParam param, Model* model, bool isAnime)
{
	SetRender3D();

	CCamera* pCamera = CCamera::GetInstance(CCamera::GetCameraKind()).get();

	param.mWorld =
		DirectX::XMMatrixScaling(param.size.x, param.size.y, param.size.z) *
		DirectX::XMMatrixRotationX(param.rotate.x) *
		DirectX::XMMatrixRotationY(param.rotate.y) *
		DirectX::XMMatrixRotationZ(param.rotate.z) *
		DirectX::XMMatrixTranslation(param.pos.x, param.pos.y, param.pos.z);

	DirectX::XMStoreFloat4x4(&param.wvp[0], DirectX::XMMatrixTranspose(param.mWorld));
	param.wvp[1] = pCamera->GetViewMatrix();		// view行列
	param.wvp[2] = pCamera->GetProjectionMatrix();	// projection行列
	// カメラ行列を設定
	Geometory::SetView(param.wvp[1]);
	Geometory::SetProjection(param.wvp[2]);


	// シェーダーへ変換行列を設定
	ShaderList::SetWVP(param.wvp); // 引数にはXMFloat4X4型の、要素数3のアドレスを渡すこと

	//モデルに使用する頂点シェーダーを設定
	model->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
	// モデルに使用する頂点ピクセルシェーダーを設定
	model->SetPixelShader(ShaderList::GetPS(ShaderList::PS_LAMBERT));

	for (UINT i = 0; i < model->GetMeshNum(); i++)
	{
		// モデルのメッシュの取得
		Model::Mesh mesh = *model->GetMesh(i);

		// メッシュに割り当てられているマテリアルを取得
		Model::Material material = *model->GetMaterial(mesh.materialID);

		// シェーダーへマテリアルを設定
		ShaderList::SetMaterial(material);

		// モデルの描画
		model->Draw(i);
	}
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

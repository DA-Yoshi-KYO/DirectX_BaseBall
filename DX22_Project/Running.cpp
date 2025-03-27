#include "Running.h"
#include "BallCount.h"
#include "Field.h"
#include "Ball.h"
#include "Main.h"
#include "Input.h"

CRunning::RunnerParam CRunning::m_tRunnerParam[(int)RunnerKind::Max] = {};

CRunning::CRunning()
{
	m_pModel = std::make_unique<Model>();
	if(!m_pModel->Load(MODELPASS("ball.obj")))ERROR_MESSAGE("");

	for (int i = 0; i < (int)RunnerKind::Max; i++)
	{
		m_tRunnerParam[i].m_bAlive = false;
		m_tRunnerParam[i].m_bRunning = false;
		m_tRunnerParam[i].m_bStayPrevBase = false;
		m_tRunnerParam[i].m_eArriveKind = RunnerKind::Max;
		m_tRunnerParam[i].m_fSpeed = 0.2f;
		m_tRunnerParam[i].m_tModelParam.size = { 5.0f,5.0f,5.0f };
		m_tRunnerParam[i].m_tModelParam.rotate = { 0.0f,0.0f,0.0f };
	}
}

CRunning::~CRunning()
{
}

void CRunning::Update()
{
	CBallCount* pBallCount = CBallCount::GetInstance().get();
	CField* pField = CField::GetInstance().get();
	CBall* pBall = CBall::GetInstance().get();
	
	switch (pBall->GetPhase())
	{
	case BallPhase::Batting:
		for (int i = 0; i < (int)RunnerKind::Max - 1; i++)
		{
			m_tRunnerParam[i].m_bAlive = pBallCount->GetBaseState(i);
			m_tRunnerParam[i].m_bStayPrevBase = false;
			m_tRunnerParam[i].m_bRunning = false;
			m_tRunnerParam[i].m_fSpeed = 0.2f;
			m_tRunnerParam[i].m_eArriveKind = (RunnerKind)i;

			if (m_tRunnerParam[i].m_bAlive)
			{
				m_tRunnerParam[i].m_tModelParam.pos = pField->GetBasePos((BaseKind)i);
			}
		}
		m_tRunnerParam[(int)RunnerKind::BatterRunner].m_tModelParam.pos = pField->GetBasePos(BaseKind::Home);
		m_tRunnerParam[(int)RunnerKind::BatterRunner].m_bAlive = true;
		m_tRunnerParam[(int)RunnerKind::BatterRunner].m_eArriveKind = RunnerKind::BatterRunner;
		break;
	case  BallPhase::InPlay:
		if (!pBallCount->GetEndInplay())
		{
			for (int i = 0; i < (int)RunnerKind::Max; i++)
			{
				if (m_tRunnerParam[i].m_bAlive)RunnerMove((RunnerKind)i);
			}
			RunnerCheck();
			BaseStateCheck();
		}
		break;
	default:
		break;
	}

}

void CRunning::Draw()
{
	CBallCount* pBallCount = CBallCount::GetInstance().get();
	CField* pField = CField::GetInstance().get();
	CBall* pBall = CBall::GetInstance().get();

	for (int i = 0; i < (int)RunnerKind::Max; i++)
	{

		if (pBall->GetPhase() == BallPhase::Batting && i == (int)RunnerKind::BatterRunner)continue;
		if (!m_tRunnerParam[i].m_bAlive)continue;

		SetModel(m_tRunnerParam[i].m_tModelParam, m_pModel.get());

	}
}

void CRunning::SetModel(ModelParam param, Model* model, bool isAnime)
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

	for (int i = 0; i < model->GetMeshNum(); i++)
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

CRunning::RunnerParam CRunning::GetRannerParam(RunnerKind kind)
{
	return m_tRunnerParam[(int)kind];
}

void CRunning::RunnerMove(RunnerKind kind)
{
	CField* pField = CField::GetInstance().get();
	CBallCount* pBallCount = CBallCount::GetInstance().get();
	CBall* pBall = CBall::GetInstance().get();

	DirectX::XMFLOAT3 fRunnerPos = m_tRunnerParam[(int)kind].m_tModelParam.pos;
	DirectX::XMFLOAT3 fTargetPos;
	DirectX::XMFLOAT3 fNowBasePos;
	DirectX::XMFLOAT3 fMovePow;
	DirectX::XMFLOAT3 fArriveEpsilon = {1.0f,1.0f,1.0f};
	DirectX::XMFLOAT3 fAdvanceEpsilon = {FLT_EPSILON,FLT_EPSILON,FLT_EPSILON };
	DirectX::XMVECTOR vecRunnerPos;
	DirectX::XMVECTOR vecTargetPos;
	DirectX::XMVECTOR vecNowBasePos;
	DirectX::XMVECTOR vecDirection;
	DirectX::XMVECTOR vecArriveEpsilon = DirectX::XMLoadFloat3(&fArriveEpsilon);
	DirectX::XMVECTOR vecAdvanceEpsilon = DirectX::XMLoadFloat3(&fAdvanceEpsilon);

	switch (m_tRunnerParam[(int)kind].m_eArriveKind)
	{
	case CRunning::RunnerKind::FirstRunner:
		fTargetPos = pField->GetBasePos(BaseKind::Second);
		fNowBasePos = pField->GetBasePos(BaseKind::First);
		vecRunnerPos = DirectX::XMLoadFloat3(&fRunnerPos);
		vecTargetPos = DirectX::XMLoadFloat3(&fTargetPos);
		vecNowBasePos = DirectX::XMLoadFloat3(&fNowBasePos);
		m_tRunnerParam[(int)kind].m_bRunning = false;
		if (IsKeyPress(VK_TAB) || !DirectX::XMVector3NearEqual(vecRunnerPos, vecNowBasePos, vecArriveEpsilon) || (m_tRunnerParam[(int)RunnerKind::BatterRunner].m_bAlive && m_tRunnerParam[(int)RunnerKind::BatterRunner].m_eArriveKind == RunnerKind::BatterRunner && !pBall->GetIsFry()))
		{
			m_tRunnerParam[(int)kind].m_bRunning = true;
			vecDirection = DirectX::XMVectorSubtract(vecTargetPos, vecRunnerPos);
			vecDirection = DirectX::XMVector3Normalize(vecDirection);
			vecDirection = DirectX::XMVectorScale(vecDirection, m_tRunnerParam[(int)kind].m_fSpeed);
			DirectX::XMStoreFloat3(&fMovePow, vecDirection);
			m_tRunnerParam[(int)kind].m_tModelParam.pos.x += fMovePow.x;
			m_tRunnerParam[(int)kind].m_tModelParam.pos.z += fMovePow.z;
			if (DirectX::XMVector3NearEqual(vecRunnerPos, vecTargetPos, vecArriveEpsilon))m_tRunnerParam[(int)kind].m_eArriveKind = RunnerKind::SecondRunner;
		}
		break;
	case CRunning::RunnerKind::SecondRunner:
		fTargetPos = pField->GetBasePos(BaseKind::Third);
		fNowBasePos = pField->GetBasePos(BaseKind::Second);
		vecRunnerPos = DirectX::XMLoadFloat3(&fRunnerPos);
		vecTargetPos = DirectX::XMLoadFloat3(&fTargetPos);
		vecNowBasePos = DirectX::XMLoadFloat3(&fNowBasePos);
		m_tRunnerParam[(int)kind].m_bRunning = false;
		if (IsKeyPress(VK_TAB) || !DirectX::XMVector3NearEqual(vecRunnerPos, vecNowBasePos, vecArriveEpsilon) || (m_tRunnerParam[(int)RunnerKind::FirstRunner].m_bAlive && m_tRunnerParam[(int)RunnerKind::FirstRunner].m_eArriveKind == RunnerKind::FirstRunner && !pBall->GetIsFry()))
		{
			m_tRunnerParam[(int)kind].m_bRunning = true;
			vecDirection = DirectX::XMVectorSubtract(vecTargetPos, vecRunnerPos);
			vecDirection = DirectX::XMVector3Normalize(vecDirection);
			vecDirection = DirectX::XMVectorScale(vecDirection, m_tRunnerParam[(int)kind].m_fSpeed);
			DirectX::XMStoreFloat3(&fMovePow, vecDirection);
			m_tRunnerParam[(int)kind].m_tModelParam.pos.x += fMovePow.x;
			m_tRunnerParam[(int)kind].m_tModelParam.pos.z += fMovePow.z;
			if (DirectX::XMVector3NearEqual(vecRunnerPos, vecTargetPos, vecArriveEpsilon))m_tRunnerParam[(int)kind].m_eArriveKind = RunnerKind::ThirdRunner;
		}
		break;
	case CRunning::RunnerKind::ThirdRunner:
		fTargetPos = pField->GetBasePos(BaseKind::Home);
		fNowBasePos = pField->GetBasePos(BaseKind::Third);
		vecRunnerPos = DirectX::XMLoadFloat3(&fRunnerPos);
		vecTargetPos = DirectX::XMLoadFloat3(&fTargetPos);
		vecNowBasePos = DirectX::XMLoadFloat3(&fNowBasePos);
		m_tRunnerParam[(int)kind].m_bRunning = false;
		if (IsKeyPress(VK_TAB) || !DirectX::XMVector3NearEqual(vecRunnerPos, vecNowBasePos, vecArriveEpsilon) || (m_tRunnerParam[(int)RunnerKind::SecondRunner].m_bAlive && m_tRunnerParam[(int)RunnerKind::SecondRunner].m_eArriveKind == RunnerKind::SecondRunner && !pBall->GetIsFry()))
		{
			m_tRunnerParam[(int)kind].m_bRunning = true;
			vecDirection = DirectX::XMVectorSubtract(vecTargetPos, vecRunnerPos);
			vecDirection = DirectX::XMVector3Normalize(vecDirection);
			vecDirection = DirectX::XMVectorScale(vecDirection, m_tRunnerParam[(int)kind].m_fSpeed);
			DirectX::XMStoreFloat3(&fMovePow, vecDirection);
			m_tRunnerParam[(int)kind].m_tModelParam.pos.x += fMovePow.x;
			m_tRunnerParam[(int)kind].m_tModelParam.pos.z += fMovePow.z;
			if (DirectX::XMVector3NearEqual(vecRunnerPos, vecTargetPos, vecArriveEpsilon))m_tRunnerParam[(int)kind].m_eArriveKind = RunnerKind::HomeIn;
		}
		break;
	case CRunning::RunnerKind::BatterRunner:
		fTargetPos = pField->GetBasePos(BaseKind::First);
		vecRunnerPos = DirectX::XMLoadFloat3(&fRunnerPos);
		vecTargetPos = DirectX::XMLoadFloat3(&fTargetPos);
		vecDirection = DirectX::XMVectorSubtract(vecTargetPos, vecRunnerPos);
		vecDirection = DirectX::XMVector3Normalize(vecDirection);
		vecDirection = DirectX::XMVectorScale(vecDirection, m_tRunnerParam[(int)kind].m_fSpeed);
		DirectX::XMStoreFloat3(&fMovePow, vecDirection);
		m_tRunnerParam[(int)kind].m_tModelParam.pos.x += fMovePow.x;
		m_tRunnerParam[(int)kind].m_tModelParam.pos.z += fMovePow.z;
		m_tRunnerParam[(int)kind].m_bRunning = true;
		if (DirectX::XMVector3NearEqual(vecRunnerPos, vecTargetPos, vecArriveEpsilon))m_tRunnerParam[(int)kind].m_eArriveKind = RunnerKind::FirstRunner;
		break;
	case CRunning::RunnerKind::HomeIn:
		if (m_tRunnerParam[(int)kind].m_bAlive)
		{
			m_tRunnerParam[(int)kind].m_bAlive = false;
			pBallCount->AddScore((int)pBallCount->GetInning());
		}
		break;
	case CRunning::RunnerKind::Max:
		break;
	default:
		break;
	}
}

void CRunning::RunnerCheck()
{
	CBallCount* pBallCount = CBallCount::GetInstance().get();

	bool bCheck = true;
	for (int i = 0; i < (int)RunnerKind::Max; i++)
	{
		if (!bCheck) break;
		if (!m_tRunnerParam[i].m_bAlive)m_tRunnerParam[i].m_bRunning = false;
		if (m_tRunnerParam[i].m_bRunning)bCheck = false;
	}
	pBallCount->SetEndInplay(CBallCount::InplayElement::Running, bCheck);
}

void CRunning::BaseStateCheck()
{
	CBallCount* pBallCount = CBallCount::GetInstance().get();
	if (pBallCount->GetEndInplay())
	{
		for (int i = 0; i < (int)RunnerKind::Max; i++)
		{
			pBallCount->SetBaseState((int)m_tRunnerParam[i].m_eArriveKind, false);
			if (m_tRunnerParam[i].m_eArriveKind == RunnerKind::HomeIn)continue;
			if(m_tRunnerParam[i].m_bAlive)pBallCount->SetBaseState((int)m_tRunnerParam[i].m_eArriveKind, true);
		}
	}
}

void CRunning::SetOut(RunnerKind kind)
{
	CBallCount* pBallCount = CBallCount::GetInstance().get();

	if (m_tRunnerParam[(int)kind].m_bAlive)
	{
		m_tRunnerParam[(int)kind].m_bAlive = false;
		pBallCount->AddOutCount();
	}
}

void CRunning::HomeRun()
{
	CBallCount* pBallCount = CBallCount::GetInstance().get();

	for (int i = 0; i < (int)RunnerKind::Max; i++)
	{
		if (m_tRunnerParam[i].m_bAlive)
		{
			pBallCount->AddScore((int)pBallCount->GetInning());
			m_tRunnerParam[i].m_bAlive = false;
			if (i == (int)RunnerKind::BatterRunner)continue;
			pBallCount->SetBaseState(i, false);
		}
	}

	for (int i = 0; i < (int)CBallCount::InplayElement::Max; i++)
	{
		pBallCount->SetEndInplay((CBallCount::InplayElement)i, true);
	}
}

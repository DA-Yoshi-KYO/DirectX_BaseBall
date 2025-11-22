#include "Running.h"
#include "BallCount.h"
#include "Ball.h"
#include "Main.h"
#include "Input.h"
#include "Fielding.h"
#include "TeamManager.h"
#include "GameManager.h"

/*
constexpr float ce_fBetweenLegnth = 20.0f;	// ランナー間距離の限界


CRunning::RunnerParam CRunning::m_tRunnerParam[(int)RunnerKind::Max] = {};
Collision::Info CRunning::m_RunnerCollision[(int)RunnerKind::Max] = {};
bool CRunning::m_bOnBase[(int)RunnerKind::Max] = {};

// ==============================
//    メモ
// ==============================
// Speed0.2f...走力G
// Speed0.35f...走力D
// Speed0.5f...走力S

CRunning::CRunning()
{
    // モデル読み込み
    m_pModel = std::make_unique<Model>();
    if(!m_pModel->Load(PATH_MODEL("ball.obj")))ERROR_MESSAGE("");

    // ランナー要素の初期化
    for (int i = 0; i < (int)RunnerKind::Max; i++)
    {
        m_tRunnerParam[i].m_bAlive = false;
        m_tRunnerParam[i].m_bRunning = false;
        m_tRunnerParam[i].m_bStayPrevBase = false;
        m_tRunnerParam[i].m_eArriveKind = CField::BaseKind::Max;
        m_tRunnerParam[i].m_fSpeed = 0.35f;
        m_tRunnerParam[i].m_tModelParam.size = { 5.0f,5.0f,5.0f };
        m_tRunnerParam[i].m_tModelParam.rotate = { 0.0f,0.0f,0.0f };
        m_RunnerCollision[i].type = Collision::eBox;
        m_RunnerCollision[i].box.center = {0.0f,0.0f,0.0f};
        m_RunnerCollision[i].box.size = m_tRunnerParam[i].m_tModelParam.size;
    }
}

CRunning::~CRunning()
{
}

void CRunning::Update(int AttackPlayer)
{
    // インスタンスの取得
    CGameManager* pGameManager = CGameManager::GetInstance();
    CTeamManager* pTeamManager = pGameManager->GetTeamManager(AttackPlayer);
    CScene* pScene = GetScene();
    CField* pField = pScene->GetGameObject<CField>();

    // バッティング時とインプレー時で処理を変える
    switch (pGameManager->GetPhase())
    {
        // バッティング時
    case GamePhase::Batting:
        // 各ランナー要素の設定
        for (int i = 0; i < (int)RunnerKind::Max - 1; i++)
        {
            m_tRunnerParam[i].m_bAlive = pGameManager->GetBaseState(i);
            m_tRunnerParam[i].m_bStayPrevBase = false;
            m_tRunnerParam[i].m_bRunning = false;
            
            m_tRunnerParam[i].m_eArriveKind = (CField::BaseKind)i;
            m_tRunnerParam[i].m_ePrevArriveKind = (CField::BaseKind)i; 
            m_bOnBase[i] = true;

            if (m_tRunnerParam[i].m_bAlive)
            {
                m_tRunnerParam[i].m_tModelParam.pos = pField->GetBasePos((CField::BaseKind)i);
                m_tRunnerParam[i].m_fSpeed = (float)pTeamManager->GetRunnerState(i + 1).m_eSpeed * (0.3f / 7.0f) + 0.2f;
            }
        }
        m_tRunnerParam[(int)RunnerKind::BatterRunner].m_tModelParam.pos = pField->GetBasePos(CField::BaseKind::Home);
        m_tRunnerParam[(int)RunnerKind::BatterRunner].m_bAlive = true;
        m_tRunnerParam[(int)RunnerKind::BatterRunner].m_eArriveKind = CField::BaseKind::Home;
        m_tRunnerParam[(int)RunnerKind::BatterRunner].m_fSpeed = (float)pTeamManager->GetTakingBatterState().m_eSpeed * (0.3f / 7.0f) + 0.2f;;
        m_bOnBase[(int)RunnerKind::BatterRunner] = false;
        break;
        // インプレー時
    case  GamePhase::InPlay:
        // インプレー終了まで処理をする
        if (!pGameManager->GetEndInplay())
        {
            // 各ランナーの移動処理
            for (int i = 0; i < (int)RunnerKind::Max; i++)
            {
                if (m_tRunnerParam[i].m_bAlive)RunnerMove((RunnerKind)i);
            }
            // ランナーが動いているかチェック
            RunnerCheck();
            // ベース上に誰かいるかチェック
            BaseStateCheck();
        }
        break;
    default:
        break;
    }
    for (int i = 0; i < (int)RunnerKind::Max; i++)
    {
        if(m_tRunnerParam[i].m_bAlive) m_RunnerCollision[i].type = Collision::eBox;
        else  m_RunnerCollision[i].type = Collision::eNone;

        m_RunnerCollision[i].box.center = m_tRunnerParam[i].m_tModelParam.pos;
        m_RunnerCollision[i].box.size = m_tRunnerParam[i].m_tModelParam.size;
    }
}

void CRunning::Draw()
{

    // ランナーの描画処理
    for (int i = 0; i < (int)RunnerKind::Max; i++)
    {
        // バッティング時のバッターランナーは描画しない
        if (pGameManager->GetPhase() == GamePhase::Batting && i == (int)RunnerKind::BatterRunner)continue;
        // ランナーがいない時は描画しない
        if (!m_tRunnerParam[i].m_bAlive)continue;

    }
}

CRunning::RunnerParam CRunning::GetRannerParam(RunnerKind kind)
{
    return m_tRunnerParam[(int)kind];
}

void CRunning::RunnerMove(RunnerKind kind, int AttackPlayer)
{
    // インスタンスの取得
    CScene* pScene = GetScene();
    CGameManager* pGameManager = CGameManager::GetInstance();
    CField* pField = pScene->GetGameObject<CField>();
    CBall* pBall = pScene->GetGameObject<CBall>();

    // 汎用的に使う変数の定義
    DirectX::XMFLOAT3 fRunnerPos = m_tRunnerParam[(int)kind].m_tModelParam.pos;		// ランナーの座標
    DirectX::XMVECTOR vecRunnerPos = DirectX::XMLoadFloat3(&fRunnerPos);			// ランナー座標のベクトル
    DirectX::XMFLOAT3 fArriveEpsilon = { 1.0f,1.0f,1.0f };							// この誤差以内だとベースに到達したものとする
    DirectX::XMVECTOR vecArriveEpsilon = DirectX::XMLoadFloat3(&fArriveEpsilon);	// この誤差以内だとベースに到達したものとするベクトル

    // 元の塁から離れているかチェック(ランナー飛び出しに使用)
    DirectX::XMFLOAT3 fPrevBase;	// ランナーが元々いたベースの座標
    DirectX::XMVECTOR vecPrevBase;	// ランナーが元々いたベースの座標のベクトル

    fPrevBase = pField->GetBasePos(m_tRunnerParam[(int)kind].m_ePrevArriveKind);
    vecPrevBase = DirectX::XMLoadFloat3(&fPrevBase);

    // フライをキャッチして以降元のベースに居れば元の塁から離れていないものとする
    if (DirectX::XMVector3NearEqual(vecRunnerPos, vecPrevBase, vecArriveEpsilon) && CFielding::GetChatchPattern() == CFielding::ChatchPattern::Fry)
    {
        m_tRunnerParam[(int)kind].m_bStayPrevBase = true;
    }
    // 打球がフライではない場合はそもそもランナー飛び出しが起き得ないものとする
    if (!pBall->GetIsFry())
    {
        m_tRunnerParam[(int)kind].m_bStayPrevBase = true;
    }

    // キー入力による移動処理
    if (IsKeyPress(AttackPlayer, Input::Y) && IsKeyPress(AttackPlayer, Input::B) &&
        m_tRunnerParam[(int)kind].m_eDirection != Direction::Stop)
    {
        // ランナーを止める
        m_tRunnerParam[(int)kind].m_eDirection = Direction::BaseBetween;
    }
    else if (IsKeyPress(AttackPlayer, Input::Y))
    {
        // ランナーを進める
        m_tRunnerParam[(int)kind].m_eDirection = Direction::Forward;
    }
    else if (IsKeyPress(AttackPlayer, Input::B))
    {
        // ランナーを戻す
        m_tRunnerParam[(int)kind].m_eDirection = Direction::Backward;
    }

    // キー入力より優先するべき処理(例外処理)
    DirectX::XMFLOAT3 fFrontRunnerPos;		// 前にいるランナーの座標
    DirectX::XMVECTOR vecFrontToRunner;		// 前にいるランナーの座標ベクトル
    DirectX::XMVECTOR vecFrontRunnerPos;	// 前にいるランナーと自分の座標ベクトルの差
    float fFrontCheck;	// 前にランナーがいると判断する距離

    DirectX::XMFLOAT3 fHomeBasePos;	// ホームベースの座標
    DirectX::XMVECTOR vecHomeBasePos;	// ホームベースの座標ベクトル
    DirectX::XMVECTOR vecHomeToThirdRunner;	// ホームベースとサードランナーの座標ベクトルの差
    float fHomeInCheck;

    // ランナーごとの例外処理
    switch (kind)
    {
    case CRunning::RunnerKind::FirstRunner:
        // 後ろにランナーがいない時は処理をしない
        if (m_tRunnerParam[(int)RunnerKind::BatterRunner].m_bAlive)
        {
            if (!pBall->GetIsFry() && m_tRunnerParam[(int)RunnerKind::BatterRunner].m_bAlive &&
                m_tRunnerParam[(int)CRunning::RunnerKind::FirstRunner].m_eArriveKind == CField::BaseKind::First)
            {
                // ボールが落ちたタイミングで手前にランナーが居た時は進むことしかできない
                m_tRunnerParam[(int)RunnerKind::FirstRunner].m_eDirection = Direction::Forward;
            }
        }

        // フライを取った時自分の塁から離れていると元の塁に戻らなければいけない
        if (!m_tRunnerParam[(int)RunnerKind::FirstRunner].m_bStayPrevBase && pBall->GetIsFry())
        {
            m_tRunnerParam[(int)RunnerKind::FirstRunner].m_eDirection = Direction::Backward;
        }

        // 前にランナーがいない時は処理をしない
        if (m_tRunnerParam[(int)RunnerKind::SecondRunner].m_bAlive)
        {
            // 一塁ランナーと二塁ランナーの距離を測る
            fFrontRunnerPos = m_tRunnerParam[(int)RunnerKind::SecondRunner].m_tModelParam.pos;
            vecFrontRunnerPos = DirectX::XMLoadFloat3(&fFrontRunnerPos);
            vecFrontToRunner = DirectX::XMVectorSubtract(vecFrontRunnerPos, vecRunnerPos);
            vecFrontToRunner = DirectX::XMVector3Length(vecFrontToRunner);
            fFrontCheck = DirectX::XMVectorGetX(vecFrontToRunner);
            
            // 自分の前にランナーがいる時は停止状態にする
            if (fFrontCheck <= ce_fBetweenLegnth)
            {
                m_tRunnerParam[(int)RunnerKind::SecondRunner].m_eDirection = Direction::BaseBetween;
            }
        }
        break;
    case CRunning::RunnerKind::SecondRunner:
        // 後ろにランナーがいない時は処理をしない
        if (m_tRunnerParam[(int)RunnerKind::FirstRunner].m_bAlive)
        {
            // ボールが落ちたタイミングで手前にランナーが居た時は進むことしかできない
            if (!pBall->GetIsFry() && m_tRunnerParam[(int)RunnerKind::FirstRunner].m_bAlive &&
                m_tRunnerParam[(int)CRunning::RunnerKind::SecondRunner].m_eArriveKind == CField::BaseKind::Second)
            {
                m_tRunnerParam[(int)RunnerKind::SecondRunner].m_eDirection = Direction::Forward;
            }
        }

        // フライを取った時自分の塁から離れていると元の塁に戻らなければいけない
        if (!m_tRunnerParam[(int)RunnerKind::SecondRunner].m_bStayPrevBase && pBall->GetIsFry())
        {
            m_tRunnerParam[(int)RunnerKind::SecondRunner].m_eDirection = Direction::Backward;
        }

        // 前にランナーがいない時は処理をしない
        if (m_tRunnerParam[(int)RunnerKind::ThirdRunner].m_bAlive)
        {
            // 二塁ランナーと三塁ランナーの距離を測る
            fFrontRunnerPos = m_tRunnerParam[(int)RunnerKind::ThirdRunner].m_tModelParam.pos;
            vecFrontRunnerPos = DirectX::XMLoadFloat3(&fFrontRunnerPos);
            vecFrontToRunner = DirectX::XMVectorSubtract(vecFrontRunnerPos, vecRunnerPos);
            vecFrontToRunner = DirectX::XMVector3Length(vecFrontToRunner);
            fFrontCheck = DirectX::XMVectorGetX(vecFrontToRunner);

            // 自分の前にランナーがいる時は停止状態にする
            if (fFrontCheck <= ce_fBetweenLegnth)
            {
                m_tRunnerParam[(int)RunnerKind::FirstRunner].m_eDirection = Direction::BaseBetween;
            }
        }
        // なにもしていない時はベースのそばから離れていない
        else
        {
            m_tRunnerParam[(int)kind].m_bStayPrevBase = true;
        }
        break;
    case CRunning::RunnerKind::ThirdRunner:
        // 後ろにランナーがいない時は処理をしない
        if (m_tRunnerParam[(int)RunnerKind::SecondRunner].m_bAlive)
        {
            // ボールが落ちたタイミングで手前にランナーが居た時は進むことしかできない
            if (!pBall->GetIsFry() && m_tRunnerParam[(int)RunnerKind::SecondRunner].m_bAlive &&
                m_tRunnerParam[(int)CRunning::RunnerKind::ThirdRunner].m_eArriveKind == CField::BaseKind::Third)
            {
                m_tRunnerParam[(int)RunnerKind::ThirdRunner].m_eDirection = Direction::Forward;
            }
        }

        // フライを取った時自分の塁から離れていると元の塁に戻らなければいけない
        if (!m_tRunnerParam[(int)RunnerKind::ThirdRunner].m_bStayPrevBase && pBall->GetIsFry())
        {
            m_tRunnerParam[(int)RunnerKind::ThirdRunner].m_eDirection = Direction::Backward;
        }

        // ランナー飛び出し時にホームインしようとしていたら直前で止める
        fHomeBasePos = pField->GetBasePos(CField::BaseKind::Home);
        vecHomeBasePos = DirectX::XMLoadFloat3(&fHomeBasePos);
        vecHomeToThirdRunner = DirectX::XMVectorSubtract(vecHomeBasePos, vecRunnerPos);
        vecHomeToThirdRunner = DirectX::XMVector3Length(vecHomeToThirdRunner);
        fHomeInCheck = DirectX::XMVectorGetX(vecHomeToThirdRunner);
        if (!m_tRunnerParam[(int)RunnerKind::ThirdRunner].m_bStayPrevBase && fHomeInCheck <= ce_fBetweenLegnth)
        {
            m_tRunnerParam[(int)RunnerKind::ThirdRunner].m_eDirection = Direction::BaseBetween;
        }
        break;
    case CRunning::RunnerKind::BatterRunner:
        // バッターランナーが一塁に向かう時は進むことしか出来ない
        if (m_tRunnerParam[(int)RunnerKind::BatterRunner].m_eArriveKind == CField::BaseKind::Home)
        {
            m_tRunnerParam[(int)RunnerKind::BatterRunner].m_eDirection = Direction::Forward;
        }
        // バッターランナーと一塁ランナーの距離を測る
        if (m_tRunnerParam[(int)RunnerKind::FirstRunner].m_bAlive)
        {
            fFrontRunnerPos = m_tRunnerParam[(int)RunnerKind::FirstRunner].m_tModelParam.pos;
            vecFrontRunnerPos = DirectX::XMLoadFloat3(&fFrontRunnerPos);
            vecFrontToRunner = DirectX::XMVectorSubtract(vecFrontRunnerPos, vecRunnerPos);
            vecFrontToRunner = DirectX::XMVector3Length(vecFrontToRunner);
            fFrontCheck = DirectX::XMVectorGetX(vecFrontToRunner);

            // 自分の前にランナーがいる時は停止状態にする
            if (fFrontCheck <= ce_fBetweenLegnth)
            {
                m_tRunnerParam[(int)RunnerKind::BatterRunner].m_eDirection = Direction::BaseBetween;
            }
        }
        break;
    default:
        break;
    }

    // 進行方向の決定
    CField::BaseKind eNextTarget;	// 次の塁
    CField::BaseKind eBackTarget;	// 前の塁
    switch (m_tRunnerParam[(int)kind].m_eArriveKind)
    {
    case CField::BaseKind::Home:
        eNextTarget = CField::BaseKind::First;
        break;
    case CField::BaseKind::First:
        eBackTarget = CField::BaseKind::First;
        eNextTarget = CField::BaseKind::Second;
        break;
    case CField::BaseKind::Second:
        eBackTarget = CField::BaseKind::First;
        eNextTarget = CField::BaseKind::Third;
        break;
    case CField::BaseKind::Third:
        eBackTarget = CField::BaseKind::Second;
        eNextTarget = CField::BaseKind::Home;
        break;
    default:
        break;
    }

    // 進行処理
    DirectX::XMFLOAT3 fTargetPos;	// 進行先の座標
    DirectX::XMVECTOR vecTargetPos;	// 進行先の座標ベクトル
    DirectX::XMVECTOR vecDirection;	// 進行方向ベクトル
    DirectX::XMFLOAT3 fMovePow;		// 移動量
    // 進行方向別に処理を行う
    switch (m_tRunnerParam[(int)kind].m_eDirection)
    {
        // 次の塁に向かう
    case CRunning::Direction::Forward:
        // 走っている状態にする
        m_tRunnerParam[(int)kind].m_bRunning = true;

        // 進行処理
        fTargetPos = pField->GetBasePos(eNextTarget);
        vecTargetPos = DirectX::XMLoadFloat3(&fTargetPos);
        vecDirection = DirectX::XMVectorSubtract(vecTargetPos, vecRunnerPos);
        vecDirection = DirectX::XMVector3Normalize(vecDirection);
        vecDirection = DirectX::XMVectorScale(vecDirection, m_tRunnerParam[(int)kind].m_fSpeed);
        DirectX::XMStoreFloat3(&fMovePow, vecDirection);
        m_tRunnerParam[(int)kind].m_tModelParam.pos.x += fMovePow.x;
        m_tRunnerParam[(int)kind].m_tModelParam.pos.z += fMovePow.z;

        // 進行先のベースに到達したら
        if (DirectX::XMVector3NearEqual(vecRunnerPos, vecTargetPos, vecArriveEpsilon))
        {
            // 停止する
            m_tRunnerParam[(int)kind].m_bRunning = false;
            m_tRunnerParam[(int)kind].m_eDirection = Direction::Stop;

            // 次の塁を更新する
            if (m_tRunnerParam[(int)kind].m_eArriveKind != CField::BaseKind::Third)
            {
                m_tRunnerParam[(int)kind].m_eArriveKind = eNextTarget;
            }
            // 元の塁がサードの場合はランナー生存を無くし、一点を追加する
            else
            {
                m_tRunnerParam[(int)kind].m_eArriveKind = CField::BaseKind::Home;
                m_tRunnerParam[(int)kind].m_bAlive = false;
                pGameManager->GetCountManager()->AddScore();
            }
        }
        break;
        // 前の塁に向かう
    case CRunning::Direction::Backward:
        // 走っている状態にする
        m_tRunnerParam[(int)kind].m_bRunning = true;

        // 進行処理
        fTargetPos = pField->GetBasePos(m_tRunnerParam[(int)kind].m_eArriveKind);
        vecTargetPos = DirectX::XMLoadFloat3(&fTargetPos);
        vecDirection = DirectX::XMVectorSubtract(vecTargetPos, vecRunnerPos);
        vecDirection = DirectX::XMVector3Normalize(vecDirection);
        vecDirection = DirectX::XMVectorScale(vecDirection, m_tRunnerParam[(int)kind].m_fSpeed);
        DirectX::XMStoreFloat3(&fMovePow, vecDirection);
        m_tRunnerParam[(int)kind].m_tModelParam.pos.x += fMovePow.x;
        m_tRunnerParam[(int)kind].m_tModelParam.pos.z += fMovePow.z;

        // 進行先のベースに到達したら
        if (DirectX::XMVector3NearEqual(vecRunnerPos, vecTargetPos, vecArriveEpsilon))
        {
            // 停止する
            m_tRunnerParam[(int)kind].m_eDirection = Direction::Stop;
            m_tRunnerParam[(int)kind].m_bRunning = false;

            // ランナーが飛び出しており、元の塁まで戻れていなかったら前の塁を更新する
            if (m_tRunnerParam[(int)kind].m_eArriveKind != m_tRunnerParam[(int)kind].m_ePrevArriveKind && CFielding::GetChatchPattern() != CFielding::ChatchPattern::Grounder)
            {
                m_tRunnerParam[(int)kind].m_eArriveKind = eBackTarget;
            }
        }
        break;
        // ベース上で止まっている
    case CRunning::Direction::Stop:
        // 走っていない状態にする
        m_tRunnerParam[(int)kind].m_bRunning = false;
        break;
        // ベース間で止まっている
    case CRunning::Direction::BaseBetween:
        // 走っていないが、インプレー中になるので形式だけ走っている状態にする
        m_tRunnerParam[(int)kind].m_bRunning = true;
    default:
        break;
    }

    // ランナーがベース上にいるかチェック
    DirectX::XMFLOAT3 fCheckBase;	// チェックするベースの座標
    DirectX::XMVECTOR vecCheckBase;	// チェックするベースの座標ベクトル
    
    // チェック状態の初期化
    m_bOnBase[(int)kind] = false;

    // 全てのベースをチェックする
    for (int i = 0; i < (int)CField::BaseKind::Max; i++)
    {
        fCheckBase = pField->GetBasePos((CField::BaseKind)i);
        vecCheckBase = DirectX::XMLoadFloat3(&fCheckBase);

        // いずれかのベース上にいたらtrueにする
        if (DirectX::XMVector3NearEqual(vecRunnerPos, vecCheckBase, vecArriveEpsilon))
        {
            m_bOnBase[(int)kind] = true;
            // チェックが終わったら処理をしない
            break;
        }
    }
}

void CRunning::RunnerCheck()
{
    // インスタンスの取得
    CGameManager* pGameManager = CGameManager::GetInstance();

    // まずどのランナーも動いていないものとする
    bool bCheck = true;
    for (int i = 0; i < (int)RunnerKind::Max; i++)
    {
        // ランナーがいなければ動いていないものとする
        if (!m_tRunnerParam[i].m_bAlive)m_tRunnerParam[i].m_bRunning = false;
        // だれか動いていたらfalseにする
        if (m_tRunnerParam[i].m_bRunning)bCheck = false;
        // 一人でも動いていたらインプレーは終わらない
        if (!bCheck) break;
    }

    // ランナーが動いていないかをセットする
    pGameManager->SetEndInplay(CGameManager::InplayElement::Running, bCheck);
}

void CRunning::BaseStateCheck()
{
    // インスタンスの取得
    CGameManager* pGameManager = CGameManager::GetInstance();

    // インプレーが終わった時にベースの確認をする
    if (pGameManager->GetEndInplay())
    {
        // インプレー前のベース状態を一度リセットする
        for (int i = 0; i < (int)CField::BaseKind::Max; i++)
        {
            pGameManager->SetBaseState(i, false);
        }
        // 全てのランナーが到達した塁をチェックする
        for (int i = 0; i < (int)RunnerKind::Max; i++)
        {
            if (m_tRunnerParam[i].m_bAlive)pGameManager->SetBaseState((int)m_tRunnerParam[i].m_eArriveKind, true);
        }
    }
}

void CRunning::SetOut(RunnerKind kind)
{
    // インスタンスの取得
    CGameManager* pGameManager = CGameManager::GetInstance();

    // ランナーがいる事の確認
    if (m_tRunnerParam[(int)kind].m_bAlive)
    {
        // ランナーを消す
        m_tRunnerParam[(int)kind].m_bAlive = false;
        // アウトカウントを増やす
        pGameManager->GetCountManager()->AddOutCount();
    }
}

void CRunning::HomeRun()
{
    // インスタンスの取得
    CGameManager* pGameManager = CGameManager::GetInstance();

    for (int i = 0; i < (int)RunnerKind::Max; i++)
    {
        if (m_tRunnerParam[i].m_bAlive)
        {
            // ランナーの数だけスコアを追加する
            pGameManager->AddScore();
            // ランナーを消す
            m_tRunnerParam[i].m_bAlive = false;
            // 塁状況をリセットする
            pGameManager->SetBaseState(i, false);
        }
    }

    // ホームラン時は即座にインプレーを終了する
    for (int i = 0; i < (int)CGameManager::InplayElement::Max; i++)
    {
        pGameManager->SetEndInplay((CGameManager::InplayElement)i, true);
    }
}

Collision::Info CRunning::GetCollision(RunnerKind kind)
{
    return m_RunnerCollision[(int)kind];
}

bool CRunning::GetOnBase(RunnerKind kind)
{
    return m_bOnBase[(int)kind];
}

*/
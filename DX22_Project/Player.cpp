#include "Player.h"
#include "Geometory.h"
#include "Sprite.h"
#include "TrailEffect.h"
#include "Minimap.h"
#include "Ball.h"
//
//constexpr float START_POS_X = 0.0f;
//constexpr float START_POS_Y = 10.0f;
//constexpr float START_POS_Z = 20.0f;
//constexpr float PLAYER_SIZE = 1.8f;
//constexpr float PLAYER_MOVE_POWER = 0.5f;
//
//#define MAX_POWER 1.0f
//
//enum E_SHOT_STEP
//{
//	SHOT_WAIT = 0,	// 球を打つのを待つ  
//	SHOT_KEEP,		// キー入力を開始
//	SHOT_RELEASE,	// キー入力をやめた(打つ) 
//};
//
//CPlayer::CPlayer()
//	: m_pCamera(nullptr)
//	, m_Move{ 0.0f,0.0f,0.0f }
//	, m_isStop(false)
//	, m_nShotStep(SHOT_WAIT)
//	, m_fPower(0.0f)
//	, m_pShadowTex(nullptr), m_shadowPos{}
//	, m_pBall(nullptr)
//
//{
//
//	// ボールの初期位置とサイズの設定
//	m_pos = { START_POS_X + WORLD_AJUST ,START_POS_Y + WORLD_AJUST, START_POS_Z + WORLD_AJUST };
//	m_size = { PLAYER_SIZE,PLAYER_SIZE * 3.0f, PLAYER_SIZE };
//
//	// ボールの当たり判定の設定
//	m_Collision.type = Collision::eBox;
//	m_Collision.box = { m_pos,m_size };
//
//	// 影のテクスチャの読み込み
//	m_pShadowTex = new Texture();
//	if (FAILED(m_pShadowTex->Create(TEXPASS("Shadow.png")))) MessageBox(NULL, "Shadow.png", "Error", MB_OK);
//
//	// 循環参照になってしまうため.cpp側でTrailEffect.hをincldueすること 
//	// 指定された数の制御点を確保して、内部にポリラインを生成 
//	m_pTrail = new TrailEffect(this);
//	m_pTrail->AddLine(20);
//}
//
//CPlayer::~CPlayer()
//{
//	// メモリの解放
//	SAFE_DELETE(m_pTrail);
//	SAFE_DELETE(m_pShadowTex);
//}
//
//void CPlayer::Update()
//{
//	// カメラが設定されてない時は処理を行わない
//	if (!m_pCamera) { return; }
//
//	// ミニマップの透明度を切り替え
//	if (IsKeyTrigger(VK_TAB))CMinimap::SwitchMinimapAplha();
//
//	// トレイルエフェクトの更新
//	m_pTrail->Update();
//
//	PlayerInput();
//
//
//	// 当たり判定情報の更新
//	m_Collision.box = { {m_pos.x,m_pos.y - 7.0f,m_pos.z},{m_size.x / 1.3f,m_size.y,m_size.z / 1.3f} };
//}
//
//void CPlayer::Draw()
//{
//	// トレイルエフェクトの各種設定
//	m_pTrail->SetView(m_pCamera->GetViewMatrix());	// view行列の設定
//	m_pTrail->SetProjection(m_pCamera->GetProjectionMatrix());	// projection行列の設定
//	m_pTrail->SetTexture(m_pShadowTex);	// テクスチャの設定
//	m_pTrail->Draw();	// トレイルエフェクトの描画
//
//	DrawShadow();	// 影の描画
//}
//
//void CPlayer::SetCamera(CCamera* camera)
//{
//	// カメラ情報の設定
//	m_pCamera = camera;
//}
//
//void CPlayer::SetModel(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 rotate, int ModelType)
//{
//
//}
//
//void CPlayer::OnCollision(Collision::Result collision)
//{
//	DirectX::XMFLOAT3 ballPos = m_pBall->GetPos();
//	// 計算に必要な情報を事前に計算
//	DirectX::XMVECTOR vHitPos = DirectX::XMLoadFloat3(&collision.point);
//	DirectX::XMVECTOR vNormal = DirectX::XMLoadFloat3(&collision.normal);
//	DirectX::XMVECTOR vMove = DirectX::XMLoadFloat3(&m_Move);
//	vNormal = DirectX::XMVector3Normalize(vNormal);
//
//	// 反射の計算
//	DirectX::XMVECTOR vDot = DirectX::XMVector3Dot(vNormal, vMove);
//	vDot = DirectX::XMVectorScale(vDot, 2.0f);
//	vDot = DirectX::XMVectorMultiply(vNormal, DirectX::XMVectorAbs(vDot));
//	vMove = DirectX::XMVectorAdd(vMove, vDot);
//	DirectX::XMStoreFloat3(&m_Move, vMove);
//
//	// 反射後の補正
//	if (collision.other.type == Collision::eBox)
//	{
//		// ボックスに衝突した場合、衝突位置の補正
//		Collision::Box other = collision.other.box;
//		if (collision.normal.x != 0.0f)
//		{
//			ballPos.x = other.center.x + collision.normal.x * (other.size.x + m_size.x) * 0.5f;
//		}
//		else if (collision.normal.y != 0.0f)
//		{
//			ballPos.y = other.center.y + collision.normal.y * (other.size.y + m_size.y) * 0.5f;
//		}
//		else
//		{
//			ballPos.z = other.center.z + collision.normal.z * (other.size.z + m_size.z) * 0.5f;
//		}
//		m_Move.x *= 0.6f;
//		m_Move.y *= 0.4f;
//		m_Move.z *= 0.6f;
//	}
//	else
//	{
//		// 斜面に衝突した場合の位置の補正
//		m_Move.x = collision.point.x + collision.normal.x * m_size.x * 0.5f;
//		m_Move.y = collision.point.y + collision.normal.y * m_size.y * 0.5f;
//		m_Move.z = collision.point.z + collision.normal.z * m_size.z * 0.5f;
//
//		m_Move.x *= 0.2f;
//		m_Move.y *= 0.5f;
//		m_Move.z *= 0.2f;
//	}
//}
//
//Collision::Info CPlayer::GetCollision()
//{
//	// 当たり判定情報の取得
//	return m_Collision;
//}
//
//void CPlayer::UpdateShot()
//{
//}
//
//void CPlayer::UpdateMove()
//{
//}
//
//void CPlayer::PlayerInput()
//{
//
//	// カメラ位置とプレイヤー位置の取得
//	DirectX::XMFLOAT3 camPos = m_pCamera->GetLook();
//	DirectX::XMVECTOR vCamPos = DirectX::XMLoadFloat3(&camPos);
//	DirectX::XMVECTOR vPos = DirectX::XMLoadFloat3(&m_pos);
//
//	// カメラからプレイヤーへの正面ベクトルを計算
//	DirectX::XMVECTOR vecForward = DirectX::XMVectorSubtract(vPos, vCamPos);
//	vecForward = DirectX::XMVector3Normalize(vecForward);
//
//	// 右方向ベクトルを計算（上方向 (0, 1, 0) と前方向の外積）
//	DirectX::XMVECTOR vecUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
//	DirectX::XMVECTOR vecRight = DirectX::XMVector3Cross(vecUp, vecForward);
//	vecRight = DirectX::XMVector3Normalize(vecRight);
//
//	// 各ベクトルをスケール調整
//	vecForward = DirectX::XMVectorScale(vecForward, PLAYER_MOVE_POWER);
//	vecRight = DirectX::XMVectorScale(vecRight, PLAYER_MOVE_POWER);
//
//	// キー入力に応じた移動処理
//	if (IsKeyPress(VK_UP))
//	{
//		m_pos.x -= DirectX::XMVectorGetX(vecForward);
//		m_pos.z -= DirectX::XMVectorGetZ(vecForward);
//	}
//	if (IsKeyPress(VK_DOWN))
//	{
//		m_pos.x += DirectX::XMVectorGetX(vecForward);
//		m_pos.z += DirectX::XMVectorGetZ(vecForward);
//	}
//	if (IsKeyPress(VK_RIGHT))
//	{
//		m_pos.x -= DirectX::XMVectorGetX(vecRight);
//		m_pos.z -= DirectX::XMVectorGetZ(vecRight);
//	}
//	if (IsKeyPress(VK_LEFT))
//	{
//		m_pos.x += DirectX::XMVectorGetX(vecRight);
//		m_pos.z += DirectX::XMVectorGetZ(vecRight);
//	}
//}
//
//void CPlayer::DrawShadow()
//{
//	// 影の大きさを計算 
//	float rate = (m_pos.y - m_shadowPos.y) / METER(4.0f); // 距離が近ければ0,遠ければ1 
//	float scale = (1.0f - rate);        // rateを0なら1、1なら0になるよう反転 
//
//	// 影を表示するための行列計算 
//	DirectX::XMFLOAT4X4 mat;
//	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale, scale, scale); // scaleを元に拡縮 
//	DirectX::XMMATRIX R = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(90.0f));  // スプライトを横に倒す 
//	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(m_shadowPos.x, m_shadowPos.y + CMETER(0.1f), m_shadowPos.z);// Zファイティング回避で少し浮かす
//	DirectX::XMMATRIX mWorld = S * R * T;
//	mWorld = DirectX::XMMatrixTranspose(mWorld);
//	DirectX::XMStoreFloat4x4(&mat, mWorld);
//
//
//	Sprite::SetWorld(mat);
//	Sprite::SetSize({scale,scale});
//	Sprite::SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, scale * 0.8f)); // 影の透明度を設定 
//	Sprite::SetTexture(m_pShadowTex);
//	Sprite::Draw();
//}
//
//void CPlayer::SetShadow(DirectX::XMFLOAT3 pos)
//{
//	m_shadowPos = pos;
//}
//
//DirectX::XMFLOAT3 CPlayer::GetPos()
//{
//	return m_pos;
//}
//
//DirectX::XMFLOAT3 CPlayer::GetMove()
//{
//	return m_Move;
//}
//
//void CPlayer::SetPos(DirectX::XMFLOAT3 pos)
//{
//	m_pos = pos;
//}
//
//void CPlayer::SetBall(CBall* ball)
//{
//	m_pBall = ball;
//}

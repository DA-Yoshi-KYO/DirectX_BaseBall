#include "CameraDebug.h"
#include "Defines.h"
#include "Oparation.h"
#include "Input.h"

CCameraDebug::CCameraDebug()
	: m_radXZ(0.0f), m_radY(DirectX::XMConvertToRadians(180.0f)), m_radius(10.0f), m_bMove(true)
{
	m_pos = { 0.0f,0.0f, 0.0f };
	m_look = { 0.0f,0.0f, 10.0f };
}

CCameraDebug::~CCameraDebug()
{
}

void CCameraDebug::Update()
{
	if (!m_bMove) return;
	// カメラの座標と注視点を使い、前方向ベクトルを取得
	DirectX::XMFLOAT3 f3Forward = m_look - m_pos;
	DirectX::XMVECTOR vForward = DirectX::XMLoadFloat3(&f3Forward);
	vForward = DirectX::XMVector3Normalize(vForward);
	DirectX::XMStoreFloat3(&f3Forward, vForward);

	// カメラの規定の上方向ベクトルを取得
	DirectX::XMFLOAT3 fUp = m_up;
	DirectX::XMVECTOR vUp = DirectX::XMLoadFloat3(&m_up);
	vUp = DirectX::XMVector3Normalize(vUp);
	DirectX::XMStoreFloat3(&fUp, vUp);

	// カメラの前方向ベクトルと上方向ベクトルの内積を使い、右方向ベクトルを取得
	DirectX::XMVECTOR vRight = DirectX::XMVector3Cross(vUp, vForward);
	DirectX::XMFLOAT3 f3Right;
	DirectX::XMStoreFloat3(&f3Right, vRight);

	// キーボード入力からVelocityを計算
	DirectX::XMFLOAT3 f3Velocity{};
	if (IsKeyPress('W'))f3Velocity += f3Forward;
	if (IsKeyPress('S'))f3Velocity -= f3Forward;
	if (IsKeyPress('D'))f3Velocity += f3Right;
	if (IsKeyPress('A'))f3Velocity -= f3Right;
	if (IsKeyPress('Q'))f3Velocity += fUp;
	if (IsKeyPress('E'))f3Velocity -= fUp;

	// 計算したVelocityを注視点に加算
	m_look += f3Velocity;

	// 右クリックした際にカーソル表示を無効化し、マウスの座標を中央に移す
	if (IsMouseButtonTrigger(MOUSEBUTTON_R))
	{
		POINT center;
		center.x = 0;
		center.y = 0;
		ShowCursor(false);
		SetMousePosition(center);
	}

	// 右クリックを押しながらカメラの回転を行う
	if (IsMouseButtonPress(MOUSEBUTTON_R))
	{
		// マウス座標から中央からの移動量を取得する
		POINT mousePos = *GetMousePosition();
		// 移動量を回転に反映させる
		m_radXZ += mousePos.x * CAMERA_SPEED;
		m_radY += mousePos.y * CAMERA_SPEED;

		// カメラの座標を中央に戻す
		POINT center;
		center.x = 0;
		center.y = 0;
		SetMousePosition(center);
	}

	// 右クリックを離した際にカーソル表示をもとに戻す
	if (IsMouseButtonRelease(MOUSEBUTTON_R))
	{
		ShowCursor(true);
	}

	// 注視点、回転、カメラの距離からカメラの座標を計算する
	m_pos.x = cosf(m_radY) * sinf(m_radXZ) * m_radius + m_look.x;
	m_pos.y = sinf(m_radY) * m_radius + m_look.y;
	m_pos.z = cosf(m_radY) * cosf(m_radXZ) * m_radius + m_look.z;
}

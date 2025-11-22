#pragma once

#include <DirectXMath.h>
#include <array>

enum CameraKind
{
	CAM_DEBUG,
	CAM_EVENT, // イベント用の定義
	CAM_BATTER,
	CAM_INPLAY,
	MAX_CAMERA // カメラ最大数 
};

class CCamera
{	
public:
	CCamera();
	virtual ~CCamera();
	virtual void Update() = 0;
	/*Getter*/
	DirectX::XMFLOAT4X4 GetViewMatrix(bool transpose = true);
	DirectX::XMFLOAT4X4 GetProjectionMatrix(bool transpose = true);
	DirectX::XMFLOAT3 GetPos() { return m_pos; }
	DirectX::XMFLOAT3 GetLook() { return m_look; }
	DirectX::XMFLOAT3 GetUp() { return m_up; }
	DirectX::XMFLOAT3 GetForward() { return m_look; }

	static CCamera* GetInstance();
	void Release();
	void SetCameraKind(CameraKind kind) { m_eKind = kind; }
protected:
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_look;
	DirectX::XMFLOAT3 m_up;
	float m_fovy;
	float m_aspect;
	float m_near;
	float m_far;
private:
	static std::array<CCamera*,(int)CameraKind::MAX_CAMERA> m_pInstance;
	static CameraKind m_eKind;

};

#pragma once

#include "GameObject.h"
#include "Input.h"
#include "Texture.h"
#include "PolylineEffect.h" 

//class CBall;
//
//class CPlayer : public CGameObject
//{
//public:
//	CPlayer();
//	~CPlayer();
//	void Update() override;
//	void Draw() override;
//	void SetCamera(CCamera* camera);
//	void SetModel(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 rotate, int ModelType = 0) override;
//	void OnCollision(Collision::Result collision) override;
//	void SetShadow(DirectX::XMFLOAT3 pos);
//	DirectX::XMFLOAT3 GetPos();
//	DirectX::XMFLOAT3 GetMove();
//	void SetPos(DirectX::XMFLOAT3 pos);
//	void SetBall(CBall* ball);
//	
//	Collision::Info GetCollision() override;
//	float GetPower() { return m_fPower; }
//private:
//
//	// 内部処理
//	void UpdateShot();
//	void UpdateMove();
//
//	void PlayerInput();
//
//	void DrawShadow();
//
//	CCamera* m_pCamera;			// カメラ情報(コンポジション)
//	CBall* m_pBall;
//	DirectX::XMFLOAT3 m_Move;	// 移動量
//	bool m_isStop;				// ボールの停止判定
//	int m_nShotStep;			// 
//	float m_fPower;				// speedをvRenに格納
//
//	Texture* m_pShadowTex;			// 影の見た目 
//	DirectX::XMFLOAT3 m_shadowPos;	// 影の位置 
//	PolylineEffect* m_pTrail; // ボールの軌跡表現 
//};
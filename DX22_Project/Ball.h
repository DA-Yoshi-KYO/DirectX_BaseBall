#pragma once

#include "GameObject.h"
#include "Camera.h"
#include "Player.h"

class CPlayer;

class CBall : public CGameObject
{
public:
	CBall();
	~CBall();
	virtual void Update();
	virtual void Draw();	
	virtual void SetModel(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 rotate, int ModelType = 0);	// モデルのセット＆描画
	virtual void OnCollision(Collision::Result collision);
	virtual Collision::Info GetCollision();
	void SetCamera(CCamera* camera);
	void SetPlayer(CPlayer* player);
	bool IsHaving();
	void ReleaseBall();
	void SetPos(DirectX::XMFLOAT3 pos);
	DirectX::XMFLOAT3 GetPos();
	int GetPhase();
	void SetPhase(int nPhase);
	void SetRandX();
private:
	float m_fRandX;
	int m_nBallPhase;
	bool m_bHave;
	Model* m_pModel;
	CCamera* m_pCamera;
	CPlayer* m_pPlayer;
};

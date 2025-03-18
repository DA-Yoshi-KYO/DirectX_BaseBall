#pragma once

#include "GameObject.h"
#include "Camera.h"
#include "Pitching.h"
#include "StrikeZone.h"
#include "Cursor.h"

constexpr DirectX::XMFLOAT3 ce_fBallPos = { 0.0f,10.0f,70.0f };
constexpr DirectX::XMFLOAT3 ce_fBallEndPos = { 0.0f,2.0f,140.0f };

class CBall : public CGameObject
{
public:
	CBall();
	virtual ~CBall();
	virtual void Update();
	virtual void Draw();

	virtual void SetModel(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 rotate, int ModelType = 0);	// モデルのセット＆描画
	virtual void OnCollision(Collision::Result collision);
	virtual Collision::Info GetCollision();

	void SetCamera(CCamera* camera);
	void SetPitching(CPitching* pitching);
	void SetCursor(CCursor* cursor);

	DirectX::XMFLOAT3 GetPos();
private:
	std::unique_ptr<Model> m_pModel;
	std::unique_ptr<CCamera> m_pCamera;
	std::unique_ptr<CPitching> m_pPitching;
	std::unique_ptr<CCursor> m_pCursor;
};

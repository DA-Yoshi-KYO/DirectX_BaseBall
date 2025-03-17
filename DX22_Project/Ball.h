#pragma once

#include "GameObject.h"
#include "Camera.h"
#include "Pitching.h"
#include "StrikeZone.h"
#include "Cursor.h"

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
private:
	std::unique_ptr<Model> m_pModel;
	std::unique_ptr<CCamera> m_pCamera;
	std::unique_ptr<CPitching> m_pPitching;
	std::unique_ptr<CCursor> m_pCursor;
};

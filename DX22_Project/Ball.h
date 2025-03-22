#pragma once

#include <memory>
#include "GameObject.h"
#include "Camera.h"
#include "StrikeZone.h"
#include "Pitching.h"
#include "PitchingCursor.h"
#include "Texture.h"

class CBatting;

constexpr DirectX::XMFLOAT3 ce_fBallPos = { 0.0f,10.0f,70.0f };
constexpr DirectX::XMFLOAT3 ce_fBallEndPos = { 0.0f,2.0f,145.0f };

class CBall : public CGameObject
{
private:
	CBall();
public:
	virtual ~CBall();
	virtual void Update();
	virtual void Draw();

	virtual void SetModel(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 rotate, int ModelType = 0);	// モデルのセット＆描画
	virtual void OnCollision(Collision::Result collision);
	virtual Collision::Info GetCollision();

	void SetCamera(CCamera* camera);
	void SetPitching(CPitching* pitching);
	void SetPitchingCursor(CPitchingCursor* cursor);
	void SetBatting(CBatting* batting);

	DirectX::XMFLOAT3 GetPos();

	static std::unique_ptr<CBall>& GetInstance();
private:
	std::unique_ptr<Texture> m_pShadow;
	std::unique_ptr<Model> m_pModel;
	std::unique_ptr<CCamera> m_pCamera;
	std::unique_ptr<CPitching> m_pPitching;
	std::unique_ptr<CPitchingCursor> m_pCursor;
	std::unique_ptr<CBatting> m_pBatting;
	int m_nPhase;
	DirectX::XMFLOAT3 m_fMove;
	DirectX::XMFLOAT3 m_fShadowPos;

	Collision::Info m_PlaneCollision;
	Collision::Info m_LucusCollision;

	void UpdateBatting();
	void UpdateInPlay();

	void DrawShadow();
};

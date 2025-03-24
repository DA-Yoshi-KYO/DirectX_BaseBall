#pragma once

#include <memory>
#include "GameObject.h"
#include "Camera.h"
#include "StrikeZone.h"
#include "Pitching.h"
#include "PitchingCursor.h"
#include "Texture.h"

class CBatting;

enum class BallPhase
{
	Batting,
	InPlay,
};

constexpr DirectX::XMFLOAT3 ce_fBallPos = { 0.0f,5.0f,70.0f };
constexpr DirectX::XMFLOAT3 ce_fBallEndPos = { 0.0f,-4.0f,145.0f };

class CBall : public CGameObject
{
private:
	CBall();
public:
	virtual ~CBall();
	virtual void Update();
	virtual void Draw();

	virtual void SetModel(ModelParam param, Model* model, bool isAnime = false);	// モデルのセット＆描画
	void OnCollision(Collision::Result collision);
	Collision::Info GetCollision();
	Collision::Info GetLineCollision();

	void SetPitching(CPitching* pitching);
	void SetPitchingCursor(CPitchingCursor* cursor);
	void SetBatting(CBatting* batting);

	DirectX::XMFLOAT3 GetPos();

	static std::unique_ptr<CBall>& GetInstance();
	BallPhase GetPhase();
private:
	std::unique_ptr<Texture> m_pShadow;
	std::unique_ptr<Model> m_pModel;
	std::unique_ptr<CPitching> m_pPitching;
	std::unique_ptr<CPitchingCursor> m_pCursor;
	std::unique_ptr<CBatting> m_pBatting;
	ModelParam m_tParam;
	int m_nPhase;
	DirectX::XMFLOAT3 m_fMove;
	DirectX::XMFLOAT3 m_fShadowPos;

	Collision::Info m_BallCollision;
	Collision::Info m_LucusCollision;

	void UpdateBatting();
	void UpdateInPlay();

	void DrawShadow();
};

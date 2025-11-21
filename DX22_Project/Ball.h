#pragma once
#include <memory>
#include "GameObject.h"
#include "Camera.h"
#include "StrikeZone.h"
#include "Pitching.h"
#include "PitchingCursor.h"

class CBatting;

constexpr DirectX::XMFLOAT3 ce_fBallPos = { 0.0f,-3.0f,65.0f };
constexpr DirectX::XMFLOAT3 ce_fBallEndPos = { 0.0f,-5.0f,148.0f };

class CBall : public CGameObject
{
public:
	CBall();
	~CBall();
	void Init() override;
	void Update() override;
	void OnCollision(CCollisionBase* other, std::string thisTag, Collision::Result result) override;

	void UpdateInPlay();
	void UpdateBatting();
	bool GetIsFryBall() { return m_bFryBall; }

private:
	Collision::Info m_LucusCollision;
	Collision::Info m_BoxCollision;
	DirectX::XMFLOAT3 m_f3Velocity;
	bool m_bFryBall;


};

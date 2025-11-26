#pragma once
#include <memory>
#include "GameObject.h"
#include "Camera.h"
#include "StrikeZone.h"
#include "Pitching.h"
#include "PitchingCursor.h"
#include "CollisionBox.h"
#include "CollisionLine.h"

class CBatting;

constexpr DirectX::XMFLOAT3 ce_fBallPos = { 0.0f,-3.0f,-145.0f };
constexpr DirectX::XMFLOAT3 ce_fJustmeetPos = { 0.0f,-2.5f, -219.0f };
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
	void SetVelocity(DirectX::XMFLOAT3 vel) { m_f3Velocity = vel; }

private:
	CCollisionLine* m_pLucusCollision;
	CCollisionBox* m_pBoxCollision;
	DirectX::XMFLOAT3 m_f3Velocity;
	bool m_bFryBall;


};

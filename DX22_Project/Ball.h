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
	bool GetIsFryChatch() { return m_bFryChatch; }
	bool GetIsFryBall() { return m_bFryBall; }
	bool GetCaught() { return m_bCaught; }
	bool GetIsInOutField() { return m_bInOutField; }
	void SetIsFryChatch(bool fryChatch) { m_bFryChatch = fryChatch; }
	void SetIsFryBall(bool isFry) { m_bFryBall = isFry; }
	void SetVelocity(DirectX::XMFLOAT3 vel) { m_f3Velocity = vel; }
	void SetPitching(float time, BenderKind kind) { m_fBallTime = time; m_eBender = kind; m_bPitched = true; }
	void SetCaught(bool isChatch);

private:
	CCollisionLine* m_pLucusCollision;
	CCollisionBox* m_pBoxCollision;
	DirectX::XMFLOAT3 m_f3Velocity;
	DirectX::XMFLOAT3 m_f3PitchingDistance;
	bool m_bFryBall;
	bool m_bFryChatch;
	bool m_bCaught;
	bool m_bPitched;
	bool m_bInOutField;
	float m_fBallTime;
	BenderKind m_eBender;

	void CheckFaul();
	void FaulCalc();

};

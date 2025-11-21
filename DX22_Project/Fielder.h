#pragma once

#include "GameObject.h"
#include "FielderData.h"
#include "CollisionBox.h"

class CFielder : public CGameObject
{
public:
	CFielder();
	~CFielder();
	void Init() override;
	void Update() override;
	void ResetPos();
	void OnCollision(CCollisionBase* other, std::string thisTag, Collision::Result result) override;
	void SetData(CFielderData* fielder, Positions position);
	bool SetBaseCoverFrag(int baseIndex, bool frag);

private:
	CFielderData* m_pFielderData;
	Positions m_ePosition;
	bool m_bIsOparation;
	bool m_bChatch;
	bool m_bMostNearToBase[(int)BaseKind::Max];
	CCollisionBox* m_pCollision;

};

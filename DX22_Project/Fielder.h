#pragma once

#include "GameObject.h"
#include "FielderData.h"
#include "CollisionBox.h"
#include "Base.h"

class CFielder : public CGameObject
{
public:
	CFielder();
	~CFielder();
	void Init() override;
	void Update() override;
	void Draw() override;
	void ResetPos();
	void OnCollision(CCollisionBase* other, std::string thisTag, Collision::Result result) override;
	void OnCollisionExit(CCollisionBase* other, std::string thisTag) override;
	void SetData(CFielderData* fielder);
	void SetData(FielderData data);
	bool SetBaseCoverFrag(int baseIndex, bool frag);
	void SetOparation(bool oparation) { m_bIsOparation = oparation; }

private:
	FielderData m_tFielderData;
	Positions m_ePosition;
	bool m_bIsOparation;
	bool m_bChatch;
	bool m_bMostNearToBase[(int)BaseKind::Max];
	CCollisionBox* m_pCollision;
	
	void BaseCover();
	void Throwing(BaseKind kind);

};

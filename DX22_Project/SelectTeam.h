#pragma once

#include "GameObject.h"
#include "TeamManager.h"

class CSelectTeam : public CGameObject
{
public:
	CSelectTeam();
	~CSelectTeam();
	void Init() {};
	virtual void Init(CTeamManager::Teams kind, int TeamNo);
	void Update() override;
	void Move(bool isRight);

	bool GetIsSelect() { return m_nIndex == CTeamManager::Teams::Max / 2; }
	bool GetIsMove() { return m_bIsMove; }

private:
	int m_nIndex;
	bool m_bIsMove;
	DirectX::XMFLOAT3 m_f3InitPos;
	DirectX::XMFLOAT3 m_f3TargetPos;
	float m_fTime;

};


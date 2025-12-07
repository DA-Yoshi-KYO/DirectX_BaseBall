#pragma once

#include "GameDirecterBase.h"
#include "Ball.h"
#include "Field.h"
#include "StrikeZone.h"
#include "Base.h"

class CFieldDirecter : public CGameDirecterBase
{
public:
	CFieldDirecter();
	~CFieldDirecter();
	void Init() override;
	void EndInplay() override;

private:
	CBall* m_pBall;
	CField* m_pField;
	CStrikeZone* m_pStrikeZone;
	std::array<CBase*, int(BaseKind::Max)> m_pBase;

};

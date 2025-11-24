#pragma once

#include "MemberSelectFieldBase.h"
#include "PitcherData.h"
#include "BenchPitcherBack.h"

class CSelectPitcherField : public CMemberSelectFieldBase
{
public:
	CSelectPitcherField();
	~CSelectPitcherField();
	void Update() override;
	void Init(int playerNo, std::list<CPitcherData*> list);

private:
	CBenchPitcherBack* m_pPitcherBack;

};

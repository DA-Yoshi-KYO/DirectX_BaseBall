#pragma once

#include "MemberSelectFieldBase.h"
#include "FielderData.h"
#include "BenchBatterBack.h"

class CSelectFielderField : public CMemberSelectFieldBase
{
public:
	CSelectFielderField();
	~CSelectFielderField();
	void Init(int playerNo, std::list<CFielderData*> list);
	void Update()override;

private:
	CBenchBatterBack* m_pBatterBack;

};

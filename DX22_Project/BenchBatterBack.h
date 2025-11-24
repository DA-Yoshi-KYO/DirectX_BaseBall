#pragma once

#include "GameObject.h"

class CBenchBatterBack : public CGameObject
{
public:
	CBenchBatterBack();
	~CBenchBatterBack();
	void Init() override;
	void Init(int inPlayerIndex);
	void Update() override;
	void SetSelectable(bool isSelect);
	bool GetSelectable() { return m_bSelectable; }
	bool GetIsMove() { return m_bMove; }

private:
	bool m_bSelectable;
	bool m_bMove;
	float m_fTime;
	int m_nPlayerIndex;


};

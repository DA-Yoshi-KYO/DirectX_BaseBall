#pragma once

#include "GameObject.h"

class CBenchPitcherBack : public CGameObject
{
public:
	CBenchPitcherBack();
	~CBenchPitcherBack();
	void Init() override;
	void Init(int inPlayerIndex);
	void Update() override;
	void SetSelectable(bool isSelect);
	bool GetSelectable() { return m_bSelectable; }
	
private:
	bool m_bSelectable;
	bool m_bMove;
	float m_fTime;
	int m_nPlayerIndex;

};

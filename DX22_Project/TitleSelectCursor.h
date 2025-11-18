#pragma once

#include "GameObject.h"

class CTitleSelectCursor : public CGameObject
{
public:
	CTitleSelectCursor();
	~CTitleSelectCursor();
	void Init() override;
	void Update() override;
	void Draw() override;
	void SetIndex(int index) { m_nIndex = index; };

private:
	int m_nIndex;
};

#pragma once

#include "GameObject.h"

class CScore : public CGameObject
{
public:
	CScore();
	~CScore();
	void Init() override;
	void AddScore();

private:
	int m_nScore;

};

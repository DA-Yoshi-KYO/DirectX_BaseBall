#pragma once

#include "GameObject.h"
#include <string>

class CMemberName : public CGameObject
{
public:
	CMemberName();
	~CMemberName();
	void Init() override {}
	void Init(std::wstring name);
};

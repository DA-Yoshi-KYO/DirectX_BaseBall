#pragma once
#include "GameObject.h"

enum class BaseKind
{
	Home,
	First,
	Second,
	Third,

	Max
};

class CBase : public CGameObject
{
public:
	CBase();
	~CBase();
	void Init() override;
	void Init(BaseKind kind);
};

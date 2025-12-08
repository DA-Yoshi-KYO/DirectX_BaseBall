#pragma once

class CGameDirecterBase
{
public:
	CGameDirecterBase();
	~CGameDirecterBase();
	virtual void Init();
	virtual void EndInplay() {};

};

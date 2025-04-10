#pragma once

#include "TeamManager.h"

class CTeamBears : public CTeamManager
{
public:
	CTeamBears();
	~CTeamBears();
	virtual void Init() override;
	virtual void Load() override;
private:

};

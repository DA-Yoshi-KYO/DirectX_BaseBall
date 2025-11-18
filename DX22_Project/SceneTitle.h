#pragma once

#include "Scene.h"
#include "TitleDirector.h"
#include <memory>

class CSceneTitle : public CScene 
{
public:
	CSceneTitle();
	~CSceneTitle();
	void Init() final;
	void Update() final;
	
private:
	std::unique_ptr<CTitleDirector> m_pDirector;

};

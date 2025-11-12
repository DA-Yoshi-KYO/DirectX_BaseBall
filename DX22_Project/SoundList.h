#pragma once

#include "Scene.h"
#include "Defines.h"
#include "Sound.h"

class CSoundList
{
private:
	CSoundList();
public:
	~CSoundList();
	void Load(CScene::SceneKind kind);
	void Destroy();

private:
	void LoadBGM(CScene::SceneKind kind);
	void LoadSE(CScene::SceneKind kind);
	std::vector<XAUDIO2_BUFFER*> m_pVecSound;
};

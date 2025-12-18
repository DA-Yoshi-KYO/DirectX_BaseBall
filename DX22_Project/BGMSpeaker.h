#pragma once

#include "GameObject.h"
#include "Audio.h"
#include "Scene.h"

class CBGMSpeaker : public CGameObject
{
public:
	CBGMSpeaker();
	~CBGMSpeaker();
	void Init() override;
	void Init(SceneKind kind);
	int Inspecter(bool isEnd = true)override;

private:
	CAudio* m_pAudio;
	char m_cPath[256];
	
};


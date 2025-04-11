#pragma once

#include "Scene.h"
#include "Texture.h"
#include "Defines.h"

class CSceneTitle : public CScene 
{
public:
	CSceneTitle();
	~CSceneTitle();
	void Update() final;
	void Draw() final;

private:
	enum class TitlePhase
	{
		Animation,
		Select
	}m_eTitlePhase;

	enum class TextureKind
	{
		Back,
		Logo,

		Max
	};
	std::unique_ptr<Texture> m_pTexture[(int)TextureKind::Max];
	SpriteParam m_tParam[(int)TextureKind::Max];
	bool m_bSelected;
private:
	void UpdateAnimation();
	void UpdateSelect();

	void ResetSpriteParam();
};

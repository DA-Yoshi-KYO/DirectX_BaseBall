#pragma once

#include "Scene.h"
#include "Texture.h"
#include "Defines.h"
#include <coroutine>

constexpr int ce_nMaxAnime = 3;

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
		Ball,
		Bat,
		Bat2,

		Max
	};
	std::unique_ptr<Texture> m_pTexture[(int)TextureKind::Max];
	SpriteParam m_tParam[(int)TextureKind::Max];
	bool m_bSelected;

	enum class AnimePhase
	{
		Ball,
		BatGrove,
		Max
	};
	bool m_bAnime[(int)AnimePhase::Max];
private:
	void UpdateAnimation();
	void UpdateSelect();

	bool BallAnimation();
	bool BatAnimation();

	void ResetSpriteParam();
};

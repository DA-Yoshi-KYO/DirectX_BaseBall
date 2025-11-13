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
		Logo,
		Start,
		End,
		Cursor,

		Max
	};
	std::unique_ptr<Texture> m_pTexture[(int)TextureKind::Max];
	ObjectParam m_tParam[(int)TextureKind::Max];
	bool m_bSelected;

	enum class AnimePhase
	{
		Ball,
		BatGrove,
		Logo,
		Buttons,

		Max
	};
	bool m_bAnime[(int)AnimePhase::Max];

	enum class SelectKind
	{
		Start,
		End
	};
	int m_nSelectKind;
	bool m_bCursorAnim;
private:
	void UpdateAnimation();
	void UpdateSelect();

	bool BallAnimation();
	bool BatAnimation();
	bool LogoAnimation();

	bool ButtonAnimation();

	void ResetSpriteParam();
};

#pragma once
#include "Scene.h"
#include "TeamManager.h"
#include "Texture.h"
#include "Defines.h"
#include "Write.h"

class CSceneMemberselect : public CScene
{
public:
	CSceneMemberselect(CTeamManager::Teams player1 , CTeamManager::Teams player2);
	virtual ~CSceneMemberselect();
	virtual void Update() final;
	virtual void Draw() final;
private:
	enum class SelectPhase
	{
		Home,
		Player1Pitcher,
		Player1Batter,
		Player1Option,
		Player2Pitcher,
		Player2Batter,
		Player2Option,
		GameOption,
	}m_ePhase;

	enum class CursorPhase
	{
		Start,
		PitcherChange,
		BatterChange,
		UserOption,
		GameOption,

		Max
	};
	int m_nCursor[Player::MaxPlayer];

	enum class TextureKind
	{
		Back,
		PlayerSeeat,
		Cursor,

		Max
	};

	std::unique_ptr<Texture> m_pTexture[(int)TextureKind::Max];
	std::vector<SpriteParam> m_tVecPitcherSpriteParam[Player::MaxPlayer];
	std::vector<SpriteParam> m_tVecBatterSpriteParam[Player::MaxPlayer];
	SpriteParam m_tBackParam;
	SpriteParam m_tCursorParam[Player::MaxPlayer];
	CWrite::FontData m_tFont;
	std::unique_ptr<CWrite> m_pWrite;

	bool m_bEnd;
	bool m_bReady[Player::MaxPlayer];

	void UpdateHome(Player player);
};

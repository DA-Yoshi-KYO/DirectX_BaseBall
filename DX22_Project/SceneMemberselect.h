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

	enum class BenchCursorPhase
	{
		Cancel,
		Accept,
		BenchBatter,
		Lineup,
		LineupNo,
		StarterPitcher,
		Relief,
	
		Max
	};
	int m_nBenchCursor[Player::MaxPlayer];
	int m_nSelect[Player::MaxPlayer];

	enum class TextureKind
	{
		Back,
		PlayerSeeat,
		Cursor,
		PositionSeeat,
		BenchPitcher,
		BenchBatter,

		Max
	};

	enum class BatterChange
	{
		None,
		PlayerChange,
		NoChange,
	}m_eBatterChange;

	std::unique_ptr<Texture> m_pTexture[(int)TextureKind::Max];
	std::vector<SpriteParam> m_tVecPitcherSpriteParam[Player::MaxPlayer];
	std::vector<SpriteParam> m_tVecBatterSpriteParam[Player::MaxPlayer];
	SpriteParam m_tBackParam;
	SpriteParam m_tCursorParam[Player::MaxPlayer];
	SpriteParam m_tPositionSpriteParam;
	SpriteParam m_tBenchPitcherSpriteParam;
	SpriteParam m_tBenchBatterSpriteParam;
	CWrite::FontData m_tFont;
	std::unique_ptr<CWrite> m_pWrite;
	std::vector<CTeamManager::PitcherState> m_tVecPrevPitcherMember;
	std::vector<CTeamManager::BatterState> m_tVecPrevBatterMember;

	DirectX::XMFLOAT4 m_fBenchOriginPos;
	bool m_bChanging;
	bool m_bChangingPitcher;
	bool m_bEnd;
	bool m_bReady[Player::MaxPlayer];
	bool m_bMemberChange;

	void SetBenchParam(Player player);
	void SetHomeParam(Player player,bool isCancel);

	void UpdateHome(Player player);
	void UpdateBenchPitcher(Player player);
	void UpdateBenchBatter(Player player);

	void DrawMain();
	void DrawBench();
	void DrawCursor();
};

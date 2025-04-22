#pragma once

#include "Scene.h"
#include "TeamManager.h"
#include "Texture.h"

class CSceneTeamSelect : public CScene
{
public:
	CSceneTeamSelect();
	virtual ~CSceneTeamSelect();
	virtual void Update() final;
	virtual void Draw() final;

	static CTeamManager::Teams GetTeam(int TeamNo);
private:
	static int m_nTeam[2];
	bool m_bCanMove[2];
	bool m_bSelected[2];
	bool m_bEnd;
	float m_fMovePow[2];
	float m_fOriginPos[2][5];
	enum class TextureKind
	{
		Back,
		Arrow,

		Max
	};
	std::unique_ptr<Texture> m_pTexture[(int)TextureKind::Max];
	SpriteParam m_tParam[(int)TextureKind::Max];
	std::unique_ptr<Texture> m_pTeamTexture[(int)CTeamManager::Teams::Max];
	SpriteParam m_tTeamParam[2][(int)CTeamManager::Teams::Max];
};
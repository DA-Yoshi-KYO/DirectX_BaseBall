#include "SceneTeamselect.h"
#include "Sprite.h"
#include "Camera.h"
#include "Input.h"
#include "TeamManager.h"

int CSceneTeamSelect::m_nTeam[2] = { 0, 0 };

constexpr int ce_nMaxRender = 5;
constexpr float ce_fTeamPosY[ce_nMaxRender] =
{
	0.0f,
	-300.0f,
	300.0f,
	-600.0f,
	600.0f,
};

constexpr float ce_fMovePos = 300.0f;
CSceneTeamSelect::CSceneTeamSelect()
	: m_pTeamTexture{}, m_pTexture{}, m_bCanMove{true,true}, m_fMovePow{}
	, m_bSelected{}, m_bEnd(false), m_fOriginPos{}
{
	m_nTeam[0] = CTeamManager::Teams::Bears;
	m_nTeam[1] = CTeamManager::Teams::Bears;

	for (int i = 0; i < (int)TextureKind::Max; i++)
	{
		m_pTexture[i] = std::make_unique<Texture>();
		m_tParam[i].pos = { 0.0f,0.0f };
		m_tParam[i].size = { 100.0f,100.0f };
		m_tParam[i].rotate = 0.0f;
		m_tParam[i].color = { 1.0f,1.0f,1.0f,1.0f };
		m_tParam[i].offsetPos = { 0.0f,0.0f };
		m_tParam[i].uvPos = { 0.0f,0.0f };
		m_tParam[i].uvSize = { 1.0f,1.0f };
		m_tParam[i].view = CCamera::Get2DViewMatrix();
		m_tParam[i].proj = CCamera::Get2DProjectionMatrix();
	}
	if(FAILED(m_pTexture[(int)TextureKind::Back]->Create(PATH_TEX("TeamSelectBack.jpg"))))ERROR_MESSAGE("TeamSelectBack.jpg");
	m_tParam[(int)TextureKind::Back].size = { SCREEN_WIDTH,SCREEN_HEIGHT };

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (int)CTeamManager::Teams::Max; j++)
		{
			if(!m_pTeamTexture[j])m_pTeamTexture[j] = std::make_unique<Texture>();
			m_tTeamParam[i][j].pos = { -400.0f + 800.0f * i,0.0f };
			m_tTeamParam[i][j].size = { 200.0f,200.0f };
			m_tTeamParam[i][j].rotate = 0.0f;
			m_tTeamParam[i][j].color = { 1.0f,1.0f,1.0f,1.0f };
			m_tTeamParam[i][j].offsetPos = { 0.0f,0.0f };
			m_tTeamParam[i][j].uvPos = { 0.0f,0.0f };
			m_tTeamParam[i][j].uvSize = { 1.0f,1.0f };
			m_tTeamParam[i][j].view = CCamera::Get2DViewMatrix();
			m_tTeamParam[i][j].proj = CCamera::Get2DProjectionMatrix();
		}
	}
	if (FAILED(m_pTeamTexture[(int)CTeamManager::Teams::Bears]->Create(PATH_TEX("Teams/Bears.png"))))ERROR_MESSAGE("Teams/Bears.png");
	if (FAILED(m_pTeamTexture[(int)CTeamManager::Teams::Rabbits]->Create(PATH_TEX("Teams/Rabbits.png"))))ERROR_MESSAGE("Teams/Rabbits.png");
	if (FAILED(m_pTeamTexture[(int)CTeamManager::Teams::Tigers]->Create(PATH_TEX("Teams/Tigers.png"))))ERROR_MESSAGE("Teams/Tigers.png");
	if (FAILED(m_pTeamTexture[(int)CTeamManager::Teams::Elephants]->Create(PATH_TEX("Teams/Elephants.png"))))ERROR_MESSAGE("Teams/Elephants.png");
	if (FAILED(m_pTeamTexture[(int)CTeamManager::Teams::Monkeys]->Create(PATH_TEX("Teams/Monkeys.png"))))ERROR_MESSAGE("Teams/Monkeys.png");

	for (int i = 0; i < 2; i++)
	{
		CTeamManager::GetInstance(i)->Init();
	}
}

CSceneTeamSelect::~CSceneTeamSelect()
{

}

void CSceneTeamSelect::Update()
{
	static float fTime[2] = {};
	static int nPosCheck[2][ce_nMaxRender] = {};

	if (!m_bEnd)
	{
		for (int teamNo = 0; teamNo < 2; teamNo++)
		{
			if (m_bCanMove[teamNo] && !m_bSelected[teamNo])
			{

				for (int i = 0; i < (int)CTeamManager::Teams::Max; i++)
				{
					// 一度範囲外に置いておく
					m_tTeamParam[teamNo][i].pos.y = FLT_MAX;
				}
				// 選択中のチーム
				nPosCheck[teamNo][0] = m_nTeam[teamNo];
				// 選択中のチームの一つ前
				// 選択中のチームが一番最初の場合、一番最後のチームを指す
				nPosCheck[teamNo][1] = m_nTeam[teamNo] == (int)CTeamManager::Teams::Bears ? (int)CTeamManager::Teams::Max - 1 : m_nTeam[teamNo] - 1;
				// 選択中のチームの二つ前
				// 選択中のチームの一つ前が一番最初の場合、一番最後のチームを指す
				nPosCheck[teamNo][3] = nPosCheck[teamNo][1] == (int)CTeamManager::Teams::Bears ? (int)CTeamManager::Teams::Max - 1 : nPosCheck[teamNo][1] - 1;
				// 選択中のチームの一つ後
				// 選択中のチームが一番最後の場合、一番最初のチームを指す
				nPosCheck[teamNo][2] = m_nTeam[teamNo] == (int)CTeamManager::Teams::Max - 1 ? (int)CTeamManager::Teams::Bears : m_nTeam[teamNo] + 1;
				// 選択中のチームの二つ後
				// 選択中のチームの一つ後が一番最後の場合、一番最初のチームを指す
				nPosCheck[teamNo][4] = nPosCheck[teamNo][2] == (int)CTeamManager::Teams::Max - 1 ? (int)CTeamManager::Teams::Bears : nPosCheck[teamNo][2] + 1;
				// 選択中のチームを真ん中、
				// 選択中のチームの前を上、
				// 選択中のチームの後を下に置く
				m_tTeamParam[teamNo][nPosCheck[teamNo][0]].pos.y = ce_fTeamPosY[0];
				m_tTeamParam[teamNo][nPosCheck[teamNo][1]].pos.y = ce_fTeamPosY[1];
				m_tTeamParam[teamNo][nPosCheck[teamNo][2]].pos.y = ce_fTeamPosY[2];
				m_tTeamParam[teamNo][nPosCheck[teamNo][3]].pos.y = ce_fTeamPosY[3];
				m_tTeamParam[teamNo][nPosCheck[teamNo][4]].pos.y = ce_fTeamPosY[4];
				if (teamNo == 0 ? IsKeyTrigger(InputPlayer1::Up) : IsKeyTrigger(InputPlayer2::Up))
				{
					m_bCanMove[teamNo] = false;
					m_fMovePow[teamNo] = -ce_fMovePos;
				}
				if (teamNo == 0 ? IsKeyTrigger(InputPlayer1::Down) : IsKeyTrigger(InputPlayer2::Down))
				{
					m_bCanMove[teamNo] = false;
					m_fMovePow[teamNo] = ce_fMovePos;
				}
				if (teamNo == 0 ? IsKeyTrigger(InputPlayer1::A) : IsKeyTrigger(InputPlayer2::A))
				{
					m_bSelected[teamNo] = true;
				}
				if (teamNo == 0 ? IsKeyTrigger(InputPlayer1::B) : IsKeyTrigger(InputPlayer2::B))
				{
					m_bEnd = true;
					SetNext(SceneKind::Title);
				}

				if (!m_bCanMove[teamNo])
				{
					for (int i = 0; i < ce_nMaxRender; i++)
					{
						m_fOriginPos[teamNo][i] = m_tTeamParam[teamNo][nPosCheck[teamNo][i]].pos.y;
					}
				}

			}
			else if (!m_bCanMove[teamNo])
			{
				constexpr float ce_fMoveTime = 1.0f;
				for (int i = 0; i < ce_nMaxRender; i++)
				{
					m_tTeamParam[teamNo][nPosCheck[teamNo][i]].pos.y = m_fMovePow[teamNo] * (fTime[teamNo] / ce_fMoveTime) + m_fOriginPos[teamNo][i];
				}

				if (fTime[teamNo] > ce_fMoveTime)
				{
					if (m_fMovePow[teamNo] < 0) m_nTeam[teamNo] = (m_nTeam[teamNo] == CTeamManager::Teams::Max - 1) ? (int)CTeamManager::Teams::Bears : m_nTeam[teamNo] + 1;
					else m_nTeam[teamNo] = (m_nTeam[teamNo] == (int)CTeamManager::Teams::Bears) ? (int)CTeamManager::Teams::Max - 1 : m_nTeam[teamNo] - 1;
					m_bCanMove[teamNo] = true;
					fTime[teamNo] = 0.0f;
					for (int i = 0; i < ce_nMaxRender; i++)
					{
						nPosCheck[teamNo][i] = 0;
					}
				}

				fTime[teamNo] += 1.0f / fFPS;
			}
			else if (m_bSelected[teamNo])
			{
				if (teamNo == 0 ? IsKeyTrigger(InputPlayer1::B) : IsKeyTrigger(InputPlayer2::B))
				{
					m_bSelected[teamNo] = false;
				}
			}
		}

		static float fStartTime = 0.0f;
		if (m_bSelected[0] && m_bSelected[1])
		{
			fStartTime += 1.0f / fFPS;
			if (fStartTime >= 3.0f)
			{
				m_bEnd = true;
				fStartTime = 0.0f;
				SetNext(SceneKind::MemberSelect);
			}
		}
		else
		{
			fStartTime = 0.0f;
		}
	}
}

void CSceneTeamSelect::Draw()
{
	for (int i = 0; i < (int)TextureKind::Max; i++)
	{
		m_tParam[i].world = CCamera::Get2DWolrdMatrix(m_tParam[i].pos, m_tParam[i].rotate);
		Sprite::SetParam(m_tParam[i]);
		Sprite::SetTexture(m_pTexture[i].get());
		Sprite::Draw();
	}

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (int)CTeamManager::Teams::Max; j++)
		{
			if (m_tTeamParam[i][j].pos.y == FLT_MAX) continue;
			m_tTeamParam[i][j].world = CCamera::Get2DWolrdMatrix(m_tTeamParam[i][j].pos, m_tTeamParam[i][j].rotate);
			Sprite::SetParam(m_tTeamParam[i][j]);
			Sprite::SetTexture(m_pTeamTexture[j].get());
			Sprite::Draw();
		}
	}
}

CTeamManager::Teams CSceneTeamSelect::GetTeam(int TeamNo)
{
	return (CTeamManager::Teams)m_nTeam[TeamNo];
}

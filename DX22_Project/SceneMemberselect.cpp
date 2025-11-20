#include "SceneMemberselect.h"
#include "Main.h"
#include "Camera.h"
#include "Sprite.h"
#include "ImGuiManager.h"
#include "Input.h"

//constexpr float ce_fCursorMove = 75.0f;
//constexpr float ce_fCursorMoveBench = 325.0f;
//constexpr DirectX::XMFLOAT4 ce_fCursorReliefOriginMove = { 325.0f, 45.0f,325.0f, 45.0f };
//constexpr DirectX::XMFLOAT2 ce_fLineupPos = { 370.0f,170.0f };
//constexpr DirectX::XMFLOAT2 ce_fLineupNamePos = { 360.0f,-195.0f };
//constexpr DirectX::XMFLOAT2 ce_fPositionPos = { 220.0f,170.0f };
//constexpr DirectX::XMFLOAT2 ce_fPositionSize = { 45.0f,45.0f };
//constexpr float ce_fLineupNameSize = 30.0f;
//constexpr DirectX::XMFLOAT4 ce_fOriginBenchPos1 = { 1000.0f,0.0f, 180.0f,0.0f };
//constexpr DirectX::XMFLOAT4 ce_fOriginBenchPos2 = { -1000.0f,0.0f, -180.0f,0.0f };


CSceneMemberselect::CSceneMemberselect(TeamKind player1, TeamKind player2)
{
	m_eTeamKind[0] = player1;
	m_eTeamKind[1] = player2;
}

CSceneMemberselect::~CSceneMemberselect()
{

}

void CSceneMemberselect::Init()
{
	m_pDirector = std::make_unique<CMemberSelectDirector>();
	m_pDirector->Init(m_eTeamKind[0], m_eTeamKind[1]);
}

void CSceneMemberselect::Update()
{
	m_pDirector->Update();
	CScene::Update();
}

/*
void CSceneMemberselect::SetBenchParam(Player player)
{

	switch (player)
	{
	case One:
		m_tBenchPitcherSpriteParam.pos = { ce_fOriginBenchPos1.x,ce_fOriginBenchPos1.y };
		m_tBenchBatterSpriteParam.pos = { ce_fOriginBenchPos1.x,ce_fOriginBenchPos1.y };
		m_tCursorParam[player].pos = { -70.0f,250.0f };

		break;
	case Two:
		m_tBenchPitcherSpriteParam.pos = { ce_fOriginBenchPos2.x,ce_fOriginBenchPos2.y };
		m_tBenchBatterSpriteParam.pos = { ce_fOriginBenchPos2.x,ce_fOriginBenchPos2.y };
		m_tCursorParam[player].pos = { -420.0f,250.0f };
		break;
	default:
		break;
	}
	m_bChanging = true;
}

void CSceneMemberselect::SetHomeParam(Player player, bool isCancel)
{
	m_ePhase = SelectPhase::Home;
	if (isCancel)
	{
		CTeamManager::GetInstance(player)->SetBatterState(m_tVecPrevBatterMember);
		CTeamManager::GetInstance(player)->SetPitcherState(m_tVecPrevPitcherMember);
	}

	switch (player)
	{
	case One:
		m_fBenchOriginPos = ce_fOriginBenchPos1;
		break;
	case Two:
		m_fBenchOriginPos = ce_fOriginBenchPos2;
		break;
	case MaxPlayer:
		break;
	default:
		break;
	}
}

void CSceneMemberselect::UpdateHome(Player player)
{
	if (player >= Player::MaxPlayer || player < Player::One)
	{
		return;
	}
	if (!m_bReady[player])
	{
		if (player == Player::One ? IsKeyTrigger(InputPlayer1::Down) : IsKeyTrigger(InputPlayer2::Down))
		{
			m_nCursor[player]++;
			if (m_nCursor[player] == (int)CSceneMemberselect::CursorPhase::Max)
			{
				m_nCursor[player] = (int)CSceneMemberselect::CursorPhase::Start;
			}
			return;
		}
		if (player == Player::One ? IsKeyTrigger(InputPlayer1::Up) : IsKeyTrigger(InputPlayer2::Up))
		{
			m_nCursor[player]--;
			if (m_nCursor[player] < (int)CSceneMemberselect::CursorPhase::Start)
			{
				m_nCursor[player] = (int)CSceneMemberselect::CursorPhase::Max - 1;
			}
			return;
		}
		if (player == Player::One ? IsKeyTrigger(InputPlayer1::A) : IsKeyTrigger(InputPlayer2::A))
		{
			switch (m_nCursor[player])
			{
			case (int)CSceneMemberselect::CursorPhase::Start: 
				m_bReady[player] = true;
				break;
			case (int)CSceneMemberselect::CursorPhase::PitcherChange:
				m_bChangingPitcher = true;
				switch (player)
				{
				case One: m_ePhase = SelectPhase::Player1Pitcher; break;
				case Two: m_ePhase = SelectPhase::Player2Pitcher; break;
				}
				SetBenchParam(player);
				break;
			case (int)CSceneMemberselect::CursorPhase::BatterChange:
				m_bChangingPitcher = false;
				switch (player)
				{
				case One: m_ePhase = SelectPhase::Player1Batter; break;
				case Two: m_ePhase = SelectPhase::Player2Batter; break;
				}
				SetBenchParam(player);
				break;
			case (int)CSceneMemberselect::CursorPhase::UserOption:
				switch (player)
				{
				case One: m_ePhase = SelectPhase::Player1Option; break;
				case Two: m_ePhase = SelectPhase::Player2Option; break;
				}
				break;
			case (int)CSceneMemberselect::CursorPhase::GameOption:
				m_ePhase = SelectPhase::GameOption;
				break;
			default:
				break;
			}
			return;
		}
		if (player == Player::One ? IsKeyTrigger(InputPlayer1::B) : IsKeyTrigger(InputPlayer2::B))
		{
			m_bEnd = true;
			SetNext(SceneKind::TeamSelect);
			return;
		}
	}
	else
	{
		if (player == Player::One ? IsKeyTrigger(InputPlayer1::B) : IsKeyTrigger(InputPlayer2::B))
		{
			m_bReady[player] = false;
			return;
		}
	}

}

void CSceneMemberselect::UpdateBenchPitcher(Player player)
{
	static float fTime = 0.0f;
	constexpr float easeMaxTime = 0.5f;
	constexpr DirectX::XMFLOAT2 fStarterMove = {312.0f,50.0f};
	constexpr DirectX::XMFLOAT2 fReliefMove = {312.0f,50.0f};
	DirectX::XMFLOAT2 fStarterOrigin;
	DirectX::XMFLOAT2 fReliefOrigin;
	std::vector<CTeamManager::PitcherState> tVecPitcherState = CTeamManager::GetInstance(player)->GetPitcherState();
	switch (player)
	{
	case One:
		fStarterOrigin = { -70.0f,138.0f };
		fReliefOrigin = { -70.0f,-80.0f };
		break;
	case Two:
		fStarterOrigin = { -420.0f, 140.0f };
		fReliefOrigin = { -420.0f, -80.0f };
		break;
	case MaxPlayer:
		break;
	default:
		break;
	}

	if (fTime > easeMaxTime)
	{
		m_tCursorParam[player].rotate = DirectX::XMConvertToRadians(fTime * 180.0f);
		m_tCursorParam[player].color.w = 1.0f;
		switch (m_nBenchCursor[player])
		{
		case (int)BenchCursorPhase::Cancel:
			if ((player == Player::One ? IsKeyTrigger(InputPlayer1::Right) : IsKeyTrigger(InputPlayer2::Right)) ||
				(player == Player::One ? IsKeyTrigger(InputPlayer1::Left) : IsKeyTrigger(InputPlayer2::Left)))
			{
				m_tCursorParam[player].pos.x += ce_fCursorMoveBench;
				m_nBenchCursor[player] = (int)BenchCursorPhase::Accept;
			}
			if (player == Player::One ? IsKeyTrigger(InputPlayer1::A) : IsKeyTrigger(InputPlayer2::A))
			{
				fTime = 0.0f;
				SetHomeParam(player,true);
			}

			if (player == Player::One ? IsKeyTrigger(InputPlayer1::B) : IsKeyTrigger(InputPlayer2::B))
			{
				fTime = 0.0f;
				SetHomeParam(player, true);
			}
			if (player == Player::One ? IsKeyTrigger(InputPlayer1::Down) : IsKeyTrigger(InputPlayer2::Down))
			{
				m_nBenchCursor[player] = (int)BenchCursorPhase::StarterPitcher;
				m_nSelect[player] = 0;
			}
			break;
		case (int)BenchCursorPhase::Accept:
			if ((player == Player::One ? IsKeyTrigger(InputPlayer1::Right) : IsKeyTrigger(InputPlayer2::Right)) ||
				(player == Player::One ? IsKeyTrigger(InputPlayer1::Left) : IsKeyTrigger(InputPlayer2::Left)))
			{
				m_tCursorParam[player].pos.x -= ce_fCursorMoveBench;
				m_nBenchCursor[player] = (int)BenchCursorPhase::Cancel;
			}
			if (player == Player::One ? IsKeyTrigger(InputPlayer1::A) : IsKeyTrigger(InputPlayer2::A))
			{
				fTime = 0.0f;
				SetHomeParam(player,false);
			}

			if (player == Player::One ? IsKeyTrigger(InputPlayer1::B) : IsKeyTrigger(InputPlayer2::B))
			{
				fTime = 0.0f;
				SetHomeParam(player,true);
			}
			if (player == Player::One ? IsKeyTrigger(InputPlayer1::Down) : IsKeyTrigger(InputPlayer2::Down))
			{
				m_nBenchCursor[player] = (int)BenchCursorPhase::StarterPitcher;
				m_nSelect[player] = 0;
			}
			break;
		case (int)BenchCursorPhase::StarterPitcher:
			m_tCursorParam[player].pos = { fStarterOrigin.x + fStarterMove.x * (m_nSelect[player] % 2),fStarterOrigin.y - fStarterMove.y * (m_nSelect[player] / 2) };
			if (player == Player::One ? IsKeyTrigger(InputPlayer1::Right) : IsKeyTrigger(InputPlayer2::Right))
			{
				if (m_nSelect[player] == 4)m_nSelect[player]--;
				else
				{
					if (m_nSelect[player] % 2 == 0)m_nSelect[player]++;
					else m_nSelect[player]--;
				}
			}
			if (player == Player::One ? IsKeyTrigger(InputPlayer1::Left) : IsKeyTrigger(InputPlayer2::Left))
			{
				if (m_nSelect[player] == 4)m_nSelect[player]--;
				else
				{
					if (m_nSelect[player] % 2 == 0)m_nSelect[player]++;
					else m_nSelect[player]--;
				}
			}
			if (player == Player::One ? IsKeyTrigger(InputPlayer1::Down) : IsKeyTrigger(InputPlayer2::Down))
			{
				switch (m_nSelect[player])
				{
				case 3:
					m_nSelect[player]++;
					break;
				case 4:
					m_nBenchCursor[player] = (int)BenchCursorPhase::Relief;
					m_nSelect[player] = 0;
					break;
				default:
					m_nSelect[player] += 2;
					break;
				}
			}

			if (player == Player::One ? IsKeyTrigger(InputPlayer1::Up) : IsKeyTrigger(InputPlayer2::Up))
			{
				if (m_nSelect[player] / 2 == 0)
				{
					m_nBenchCursor[player] = (int)BenchCursorPhase::Cancel;
					switch (player)
					{
					case One:
						m_tCursorParam[player].pos = { -70.0f,250.0f };
						break;
					case Two:
						m_tCursorParam[player].pos = { -420.0f,250.0f };
						break;
					case MaxPlayer:
						break;
					default:
						break;
					}
				}
				else m_nSelect[player] -= 2;
			}
			if (player == Player::One ? IsKeyTrigger(InputPlayer1::B) : IsKeyTrigger(InputPlayer2::B))
			{
				m_nBenchCursor[player] = (int)BenchCursorPhase::Cancel;
				switch (player)
				{
				case One:
					m_tCursorParam[player].pos = { -70.0f,250.0f };
					break;
				case Two:
					m_tCursorParam[player].pos = { -420.0f,250.0f };
					break;
				case MaxPlayer:
					break;
				default:
					break;
				}
			}
			if (player == Player::One ? IsKeyTrigger(InputPlayer1::A) : IsKeyTrigger(InputPlayer2::A))
			{
				CTeamManager::GetInstance(player)->ResetStarter(m_nSelect[player]);
			}
			break;
		case (int)BenchCursorPhase::Relief:
			m_tCursorParam[player].pos = { fReliefOrigin.x + fReliefMove.x * (m_nSelect[player] % 2),fReliefOrigin.y - fReliefMove.y * (m_nSelect[player] / 2) };

			if (player == Player::One ? IsKeyTrigger(InputPlayer1::Right) : IsKeyTrigger(InputPlayer2::Right))
			{
				if (m_nSelect[player] == 6)m_nSelect[player]--;
				else
				{
					if (m_nSelect[player] % 2 == 0)m_nSelect[player]++;
					else m_nSelect[player]--;
				}
			}
			if (player == Player::One ? IsKeyTrigger(InputPlayer1::Left) : IsKeyTrigger(InputPlayer2::Left))
			{
				if (m_nSelect[player] == 6)m_nSelect[player]--;
				else
				{
					if (m_nSelect[player] % 2 == 0)m_nSelect[player]++;
					else m_nSelect[player]--;
				}
			}
			if (player == Player::One ? IsKeyTrigger(InputPlayer1::Down) : IsKeyTrigger(InputPlayer2::Down))
			{
				switch (m_nSelect[player])
				{
				case 5:
					m_nSelect[player]++;
					break;
				case 6:
					break;
				default:
					m_nSelect[player] += 2;
					break;
				}
			}

			if (player == Player::One ? IsKeyTrigger(InputPlayer1::Up) : IsKeyTrigger(InputPlayer2::Up))
			{
				if (m_nSelect[player] / 2 == 0)
				{
					m_nBenchCursor[player] = (int)BenchCursorPhase::StarterPitcher;
					m_nSelect[player] = 4;
				}
				else m_nSelect[player] -= 2;
			}
			if (player == Player::One ? IsKeyTrigger(InputPlayer1::B) : IsKeyTrigger(InputPlayer2::B))
			{
				m_nBenchCursor[player] = (int)BenchCursorPhase::Cancel;
				switch (player)
				{
				case One:
					m_tCursorParam[player].pos = { -70.0f,250.0f };
					break;
				case Two:
					m_tCursorParam[player].pos = { -420.0f,250.0f };
					break;
				case MaxPlayer:
					break;
				default:
					break;
				}
			}
			break;
		default:
			break;
		}



	}
	else
	{
		switch (player)
		{
		case One:
			m_tBenchPitcherSpriteParam.pos.x = easeOutBack(fTime, easeMaxTime, ce_fOriginBenchPos1.x, ce_fOriginBenchPos1.z, 3.0f);
			break;
		case Two:
			m_tBenchPitcherSpriteParam.pos.x = easeOutBack(fTime, easeMaxTime, ce_fOriginBenchPos2.x, ce_fOriginBenchPos2.z, 3.0f);
			break;
		case MaxPlayer:
			break;
		default:
			break;
		}
		m_tCursorParam[player].color.w = 0.0f;

	}
	fTime += 1.0f / fFPS;
}

void CSceneMemberselect::UpdateBenchBatter(Player player)
{
	static float fTime = 0.0f;
	constexpr float easeMaxTime = 0.5f;
	constexpr DirectX::XMFLOAT2 fBenchBatterMove = { 312.0f,50.0f };
	constexpr DirectX::XMFLOAT2 fLineupNoMove = { 0.0f,48.0f };
	constexpr DirectX::XMFLOAT2 fLineupMove = { 0.0f,48.0f };
	DirectX::XMFLOAT2 fBenchBatterOrigin;
	DirectX::XMFLOAT2 fLineupNoOrigin;
	DirectX::XMFLOAT2 fLineupOrigin;
	auto& spPlayer = CTeamManager::GetInstance(player);
	CTeamManager* pPlayer = spPlayer.get();
	std::vector<CTeamManager::BatterState> tVecState = pPlayer->GetBatterState();;
	switch (player)
	{
	case One:
		fBenchBatterOrigin = { -70.0f,138.0f };
		fLineupNoOrigin = { -410.0f,170.0f };
		fLineupOrigin = { -465.0f,170.0f };
		break;
	case Two:
		fBenchBatterOrigin = { -420.0f, 138.0f };
		fLineupNoOrigin = { 272.0, 170.0f };
		fLineupOrigin = { 220.0, 170.0f };
		break;
	case MaxPlayer:
		break;
	default:
		break;
	}
	struct ChangePlayer
	{
		int m_nNo;
		BenchCursorPhase m_ePhase;
	};
	static ChangePlayer tChangePlayer[2] = { 
		{-1,BenchCursorPhase::Cancel },
		{-1,BenchCursorPhase::Cancel }
	};


	if (fTime > easeMaxTime)
	{
		m_tCursorParam[player].rotate = DirectX::XMConvertToRadians(fTime * 180.0f);
		m_tCursorParam[player].color.w = 1.0f;
		switch (m_nBenchCursor[player])
		{
		case (int)BenchCursorPhase::Cancel:
			tChangePlayer[2] = { -1,BenchCursorPhase::Cancel };
			if ((player == Player::One ? IsKeyTrigger(InputPlayer1::Right) : IsKeyTrigger(InputPlayer2::Right)) ||
				(player == Player::One ? IsKeyTrigger(InputPlayer1::Left) : IsKeyTrigger(InputPlayer2::Left)))
			{
				m_tCursorParam[player].pos.x += ce_fCursorMoveBench;
				m_nBenchCursor[player] = (int)BenchCursorPhase::Accept;
			}
			if (player == Player::One ? IsKeyTrigger(InputPlayer1::A) : IsKeyTrigger(InputPlayer2::A))
			{
				fTime = 0.0f;
				SetHomeParam(player, true);
			}

			if (player == Player::One ? IsKeyTrigger(InputPlayer1::B) : IsKeyTrigger(InputPlayer2::B))
			{
				fTime = 0.0f;
				SetHomeParam(player, true);
			}
			if (player == Player::One ? IsKeyTrigger(InputPlayer1::Down) : IsKeyTrigger(InputPlayer2::Down))
			{
				m_nBenchCursor[player] = (int)BenchCursorPhase::BenchBatter;
				m_nSelect[player] = 0;
			}
			break;
		case (int)BenchCursorPhase::Accept:
			tChangePlayer[2] = { -1,BenchCursorPhase::Cancel };
			if ((player == Player::One ? IsKeyTrigger(InputPlayer1::Right) : IsKeyTrigger(InputPlayer2::Right)) ||
				(player == Player::One ? IsKeyTrigger(InputPlayer1::Left) : IsKeyTrigger(InputPlayer2::Left)))
			{
				m_tCursorParam[player].pos.x -= ce_fCursorMoveBench;
				m_nBenchCursor[player] = (int)BenchCursorPhase::Cancel;
			}
			if (player == Player::One ? IsKeyTrigger(InputPlayer1::A) : IsKeyTrigger(InputPlayer2::A))
			{
				fTime = 0.0f;
				SetHomeParam(player, false);
			}

			if (player == Player::One ? IsKeyTrigger(InputPlayer1::B) : IsKeyTrigger(InputPlayer2::B))
			{
				fTime = 0.0f;
				SetHomeParam(player, true);
			}
			if (player == Player::One ? IsKeyTrigger(InputPlayer1::Down) : IsKeyTrigger(InputPlayer2::Down))
			{
				m_nBenchCursor[player] = (int)BenchCursorPhase::BenchBatter;
				m_nSelect[player] = 0;
			}
			break;
		case (int)BenchCursorPhase::BenchBatter:
			m_tCursorParam[player].pos = { fBenchBatterOrigin.x + fBenchBatterMove.x * (m_nSelect[player] % 2),fBenchBatterOrigin.y - fBenchBatterMove.y * (m_nSelect[player] / 2) };
			if (player == Player::One ? IsKeyTrigger(InputPlayer1::Right) : IsKeyTrigger(InputPlayer2::Right))
			{
				if (m_nSelect[player] % 2 == 0 && m_nSelect[player] != 6)m_nSelect[player]++;
				else if(player == Player::Two)
				{
					 m_nBenchCursor[player] = (int)BenchCursorPhase::LineupNo;
					 m_nSelect[player] = 0;
				}
			}
			if (player == Player::One ? IsKeyTrigger(InputPlayer1::Left) : IsKeyTrigger(InputPlayer2::Left))
			{
				if (m_nSelect[player] % 2 == 0)
				{
					if (player == Player::One) m_nBenchCursor[player] = (int)BenchCursorPhase::LineupNo;
					m_nSelect[player] = 0;
				}
				else m_nSelect[player]--;
			}
			if (player == Player::One ? IsKeyTrigger(InputPlayer1::Down) : IsKeyTrigger(InputPlayer2::Down))
			{
				switch (m_nSelect[player])
				{
				case 5:
					m_nSelect[player]++;
					break;
				case 6:
					break;
				default:
					m_nSelect[player] += 2;
					break;
				}
			}

			if (player == Player::One ? IsKeyTrigger(InputPlayer1::Up) : IsKeyTrigger(InputPlayer2::Up))
			{
				if (m_nSelect[player] / 2 == 0)
				{
					m_nBenchCursor[player] = (int)BenchCursorPhase::Cancel;
					switch (player)
					{
					case One:
						m_tCursorParam[player].pos = { -70.0f,250.0f };
						break;
					case Two:
						m_tCursorParam[player].pos = { -420.0f,250.0f };
						break;
					case MaxPlayer:
						break;
					default:
						break;
					}
				}
				else m_nSelect[player] -= 2;
			}
			if (player == Player::One ? IsKeyTrigger(InputPlayer1::B) : IsKeyTrigger(InputPlayer2::B))
			{
				m_nBenchCursor[player] = (int)BenchCursorPhase::Cancel;
				switch (player)
				{
				case One:
					m_tCursorParam[player].pos = { -70.0f,250.0f };
					break;
				case Two:
					m_tCursorParam[player].pos = { -420.0f,250.0f };
					break;
				case MaxPlayer:
					break;
				default:
					break;
				}
			}
			if (player == Player::One ? IsKeyTrigger(InputPlayer1::A) : IsKeyTrigger(InputPlayer2::A))
			{
				if (tChangePlayer[0].m_nNo == -1)
				{
					tChangePlayer[0].m_nNo = m_nSelect[player];
					tChangePlayer[0].m_ePhase = (BenchCursorPhase)m_nBenchCursor[player];
				}
				else
				{
					tChangePlayer[1].m_nNo = m_nSelect[player];
					tChangePlayer[1].m_ePhase = (BenchCursorPhase)m_nBenchCursor[player];
				}
			}
			break;
		case (int)BenchCursorPhase::LineupNo:
			m_tCursorParam[player].pos = { fLineupNoOrigin.x ,fLineupNoOrigin.y - fLineupNoMove.y * m_nSelect[player] };

			if (player == Player::One ? IsKeyTrigger(InputPlayer1::Right) : IsKeyTrigger(InputPlayer2::Left))
			{
				m_nBenchCursor[player] = (int)BenchCursorPhase::BenchBatter;
			
				m_nSelect[player] = 0;
			}
			if (player == Player::One ? IsKeyTrigger(InputPlayer1::Left) : IsKeyTrigger(InputPlayer2::Left))
			{
				m_nBenchCursor[player] = (int)BenchCursorPhase::Lineup;

				m_nSelect[player] = 0;
			}
			if (player == Player::One ? IsKeyTrigger(InputPlayer1::Down) : IsKeyTrigger(InputPlayer2::Down))
			{
				m_nSelect[player]++;
				if (m_nSelect[player] >= 9)
				{
					m_nSelect[player] = 0;
				}
			}

			if (player == Player::One ? IsKeyTrigger(InputPlayer1::Up) : IsKeyTrigger(InputPlayer2::Up))
			{
				m_nSelect[player]--;
				if (m_nSelect[player] < 0)
				{
					m_nSelect[player] = 8;
				}
			}
			if (player == Player::One ? IsKeyTrigger(InputPlayer1::B) : IsKeyTrigger(InputPlayer2::B))
			{
				m_nBenchCursor[player] = (int)BenchCursorPhase::Cancel;
				switch (player)
				{
				case One:
					m_tCursorParam[player].pos = { -70.0f,250.0f };
					break;
				case Two:
					m_tCursorParam[player].pos = { -420.0f,250.0f };
					break;
				case MaxPlayer:
					break;
				default:
					break;
				}
			}
			if (player == Player::One ? IsKeyTrigger(InputPlayer1::A) : IsKeyTrigger(InputPlayer2::A))
			{
				if (tChangePlayer[0].m_nNo == -1)
				{
					tChangePlayer[0].m_nNo = m_nSelect[player];
					tChangePlayer[0].m_ePhase = (BenchCursorPhase)m_nBenchCursor[player];
				}
				else
				{
					tChangePlayer[1].m_nNo = m_nSelect[player];
					tChangePlayer[1].m_ePhase = (BenchCursorPhase)m_nBenchCursor[player];
				}
			}
			break;
		case (int)BenchCursorPhase::Lineup:
			m_tCursorParam[player].pos = { fLineupOrigin.x ,fLineupOrigin.y - fLineupMove.y * m_nSelect[player] };
			if (player == Player::One ? IsKeyTrigger(InputPlayer1::Right) : IsKeyTrigger(InputPlayer2::Right))
			{
				m_nBenchCursor[player] = (int)BenchCursorPhase::LineupNo;
				m_nSelect[player] = 0;
			}
			if (player == Player::One ? false : IsKeyTrigger(InputPlayer2::Left))
			{
				m_nBenchCursor[player] = (int)BenchCursorPhase::BenchBatter;
				m_nSelect[player] = 0;
			}
			if (player == Player::One ? IsKeyTrigger(InputPlayer1::Down) : IsKeyTrigger(InputPlayer2::Down))
			{
				m_nSelect[player]++;
				if (m_nSelect[player] >= 9)
				{
					m_nSelect[player] = 0;
				}
			}

			if (player == Player::One ? IsKeyTrigger(InputPlayer1::Up) : IsKeyTrigger(InputPlayer2::Up))
			{
				m_nSelect[player]--;
				if (m_nSelect[player] < 0)
				{
					m_nSelect[player] = 8;
				}
			}
			if (player == Player::One ? IsKeyTrigger(InputPlayer1::B) : IsKeyTrigger(InputPlayer2::B))
			{
				m_nBenchCursor[player] = (int)BenchCursorPhase::Cancel;
				switch (player)
				{
				case One:
					m_tCursorParam[player].pos = { -70.0f,250.0f };
					break;
				case Two:
					m_tCursorParam[player].pos = { -420.0f,250.0f };
					break;
				case MaxPlayer:
					break;
				default:
					break;
				}
			}
			if (player == Player::One ? IsKeyTrigger(InputPlayer1::A) : IsKeyTrigger(InputPlayer2::A))
			{
				if (tChangePlayer[0].m_nNo == -1)
				{
					tChangePlayer[0].m_nNo = m_nSelect[player];
					tChangePlayer[0].m_ePhase = (BenchCursorPhase)m_nBenchCursor[player];
				}
				else
				{
					tChangePlayer[1].m_nNo = m_nSelect[player];
					tChangePlayer[1].m_ePhase = (BenchCursorPhase)m_nBenchCursor[player];
				}
			}
			break;
		default:
			break;
		}

		if (tChangePlayer[0].m_nNo != -1 && tChangePlayer[1].m_nNo != -1)
		{
			if (tChangePlayer[0].m_ePhase == BenchCursorPhase::Lineup && tChangePlayer[1].m_ePhase == BenchCursorPhase::Lineup)
			{
				auto itrBatter1 = tVecState.begin();
				auto itrBatter2 = tVecState.begin();
				for (int i = 0; i < Player::MaxPlayer; i++)
				{
					for (auto itr = tVecState.begin(); itr < tVecState.end(); itr++)
					{
						if (itr->m_nLineupNo == tChangePlayer[i].m_nNo + 1)
						{
							switch (i)
							{
							case 0:
								itrBatter1 = itr;
								break;
							case 1:
								itrBatter2 = itr;
								break;
							default:
								break;
							}
							break;
						}
					}
				}
				CTeamManager::GetInstance(player)->ResetFielding(itrBatter1->m_eFieldingNo, itrBatter2->m_eFieldingNo);
			}
			else
			{
				if (tChangePlayer[0].m_ePhase == BenchCursorPhase::LineupNo && tChangePlayer[1].m_ePhase == BenchCursorPhase::LineupNo)
				{
					CTeamManager::GetInstance(player)->ResetLineupNo(tChangePlayer[0].m_nNo + 1, tChangePlayer[1].m_nNo + 1);
				}
				else if ((tChangePlayer[0].m_ePhase == BenchCursorPhase::BenchBatter && tChangePlayer[1].m_ePhase == BenchCursorPhase::LineupNo) ||
					(tChangePlayer[0].m_ePhase == BenchCursorPhase::LineupNo && tChangePlayer[1].m_ePhase == BenchCursorPhase::BenchBatter))
				{
					if (tChangePlayer[0].m_ePhase == BenchCursorPhase::BenchBatter) CTeamManager::GetInstance(player)->ResetLineup(tChangePlayer[0].m_nNo + 1, tChangePlayer[1].m_nNo + 1);
					else CTeamManager::GetInstance(player)->ResetLineup(tChangePlayer[1].m_nNo + 1, tChangePlayer[0].m_nNo + 1);
				}
			}
			tChangePlayer[0].m_nNo = -1;
			tChangePlayer[1].m_nNo = -1;
		}


	}
	else
	{
		switch (player)
		{
		case One:
			m_tBenchBatterSpriteParam.pos.x = easeOutBack(fTime, easeMaxTime, ce_fOriginBenchPos1.x, ce_fOriginBenchPos1.z, 3.0f);
			break;
		case Two:
			m_tBenchBatterSpriteParam.pos.x = easeOutBack(fTime, easeMaxTime, ce_fOriginBenchPos2.x, ce_fOriginBenchPos2.z, 3.0f);
			break;
		case MaxPlayer:
			break;
		default:
			break;
		}
		m_tCursorParam[player].color.w = 0.0f;

	}
	fTime += 1.0f / fFPS;
}

void CSceneMemberselect::DrawMain()
{
	auto& spPlayer1 = CTeamManager::GetInstance(Player::One);
	CTeamManager* pPlayer1 = spPlayer1.get();
	auto& spPlayer2 = CTeamManager::GetInstance(Player::Two);
	CTeamManager* pPlayer2 = spPlayer2.get();

	//// 選択肢文字の描画
	//DirectX::XMFLOAT2 fStringPos[(int)CursorPhase::Max] = {
	//	{0.0f,-270.0f + ce_fCursorMove * (int)CursorPhase::Start},
	//	{0.0f,-270.0f + ce_fCursorMove * (int)CursorPhase::PitcherChange},
	//	{0.0f,-270.0f + ce_fCursorMove * (int)CursorPhase::BatterChange},
	//	{0.0f,-270.0f + ce_fCursorMove * (int)CursorPhase::UserOption},
	//	{0.0f,-270.0f + ce_fCursorMove * (int)CursorPhase::GameOption},
	//};
	//m_tFont.m_fFontSize = 30.0f;
	//m_pWrite->SetFont(m_tFont);
	//m_pWrite->DrawString("試合開始", fStringPos[(int)CursorPhase::Start], D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);
	//m_pWrite->DrawString("投手変更", fStringPos[(int)CursorPhase::PitcherChange], D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);
	//m_pWrite->DrawString("野手変更", fStringPos[(int)CursorPhase::BatterChange], D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);
	//m_pWrite->DrawString("操作設定", fStringPos[(int)CursorPhase::UserOption], D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);
	//m_pWrite->DrawString("試合設定", fStringPos[(int)CursorPhase::GameOption], D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);


	//m_tFont.m_fFontSize = ce_fLineupNameSize;
	//m_pWrite->SetFont(m_tFont);
	std::vector<CTeamManager::BatterState> tVecPlayer1Batter = pPlayer1->GetBatterState();

	// スタメン選手のポジションの描画

	int i = 0;
	for (auto itr = tVecPlayer1Batter.begin(); itr != tVecPlayer1Batter.end(); itr++, i++)
	{
		if (itr->m_nLineupNo == 0) continue;

		m_tVecBatterSpriteParam[Player::One][i].pos = { -ce_fLineupPos.x + 55.0f,ce_fLineupPos.y - (ce_fLineupSize.y + 3.0f) * (itr->m_nLineupNo - 1) };
		m_tVecBatterSpriteParam[Player::One][i].world = CCamera::Get2DWolrdMatrix(m_tVecBatterSpriteParam[Player::One][i].pos, m_tVecBatterSpriteParam[Player::One][i].rotate);
		Sprite::SetParam(m_tVecBatterSpriteParam[Player::One][i]);
		Sprite::SetTexture(m_pTexture[(int)TextureKind::PlayerSeeat].get());
		Sprite::Draw();

		m_tPositionSpriteParam.pos = { -ce_fPositionPos.x - 245.0f,ce_fPositionPos.y - (ce_fPositionSize.y + 3.0f) * (itr->m_nLineupNo - 1) };
		m_tPositionSpriteParam.uvPos.x = (float)((itr->m_eFieldingNo - 1) % ce_nSheetSplit) / (float)ce_nSheetSplit;
		m_tPositionSpriteParam.uvPos.y = (float)((itr->m_eFieldingNo - 1) / ce_nSheetSplit) / (float)ce_nSheetSplit;
		m_tPositionSpriteParam.world = CCamera::Get2DWolrdMatrix(m_tPositionSpriteParam.pos, m_tPositionSpriteParam.rotate);
		Sprite::SetParam(m_tPositionSpriteParam);
		Sprite::SetTexture(m_pTexture[(int)TextureKind::PositionSeeat].get());
		Sprite::Draw();

		DirectX::XMFLOAT2 fNamePos = { -ce_fLineupNamePos.x + 40.0f,ce_fLineupNamePos.y + (itr->m_nLineupNo - 1) * 48.0f };
		m_pWrite->DrawWString(itr->m_sName, fNamePos, D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);
	}

	std::vector<CTeamManager::BatterState> tVecPlayer2Batter = pPlayer2->GetBatterState();
	i = 0;
	for (auto itr = tVecPlayer2Batter.begin(); itr != tVecPlayer2Batter.end(); itr++, i++)
	{
		if (itr->m_nLineupNo == 0) continue;

		m_tVecBatterSpriteParam[Player::Two][i].pos = { ce_fLineupPos.x,ce_fLineupPos.y - (ce_fLineupSize.y + 3.0f) * (itr->m_nLineupNo - 1) };
		m_tVecBatterSpriteParam[Player::Two][i].world = CCamera::Get2DWolrdMatrix(m_tVecBatterSpriteParam[Player::Two][i].pos, m_tVecBatterSpriteParam[Player::Two][i].rotate);
		Sprite::SetParam(m_tVecBatterSpriteParam[Player::Two][i]);
		Sprite::SetTexture(m_pTexture[(int)TextureKind::PlayerSeeat].get());
		Sprite::Draw();

		m_tPositionSpriteParam.pos = { ce_fPositionPos.x,ce_fPositionPos.y - (ce_fPositionSize.y + 3.0f) * (itr->m_nLineupNo - 1) };
		m_tPositionSpriteParam.uvPos.x = (float)((itr->m_eFieldingNo - 1) % ce_nSheetSplit) / (float)ce_nSheetSplit;
		m_tPositionSpriteParam.uvPos.y = (float)((itr->m_eFieldingNo - 1) / ce_nSheetSplit) / (float)ce_nSheetSplit;
		m_tPositionSpriteParam.world = CCamera::Get2DWolrdMatrix(m_tPositionSpriteParam.pos, m_tPositionSpriteParam.rotate);
		Sprite::SetParam(m_tPositionSpriteParam);
		Sprite::SetTexture(m_pTexture[(int)TextureKind::PositionSeeat].get());
		Sprite::Draw();

		DirectX::XMFLOAT2 fNamePos = { ce_fLineupNamePos.x,ce_fLineupNamePos.y + (itr->m_nLineupNo - 1) * 48.0f };
		m_pWrite->DrawWString(itr->m_sName, fNamePos, D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);
	}

	std::vector<CTeamManager::PitcherState> tVecPlayer1Pitcher = pPlayer1->GetPitcherState();
	i = 0;
	for (auto itr = tVecPlayer1Pitcher.begin(); itr != tVecPlayer1Pitcher.end(); itr++, i++)
	{
		if (itr->m_bEntry)
		{
			m_tVecPitcherSpriteParam[Player::One][i].pos = { -ce_fLineupPos.x + 55.0f,ce_fLineupPos.y - (ce_fLineupSize.y + 3.0f) * 9 };
			m_tVecPitcherSpriteParam[Player::One][i].world = CCamera::Get2DWolrdMatrix(m_tVecPitcherSpriteParam[Player::One][i].pos, m_tVecPitcherSpriteParam[Player::One][i].rotate);
			Sprite::SetParam(m_tVecPitcherSpriteParam[Player::One][i]);
			Sprite::SetTexture(m_pTexture[(int)TextureKind::PlayerSeeat].get());
			Sprite::Draw();

			m_tPositionSpriteParam.pos = { -ce_fPositionPos.x - 245.0f,ce_fPositionPos.y - (ce_fPositionSize.y + 3.0f) * 9 };
			m_tPositionSpriteParam.uvPos.x = (float)((itr->m_eFieldingNo - 1) % ce_nSheetSplit) / (float)ce_nSheetSplit;
			m_tPositionSpriteParam.uvPos.y = (float)((itr->m_eFieldingNo - 1) / ce_nSheetSplit) / (float)ce_nSheetSplit;
			m_tPositionSpriteParam.world = CCamera::Get2DWolrdMatrix(m_tPositionSpriteParam.pos, m_tPositionSpriteParam.rotate);
			Sprite::SetParam(m_tPositionSpriteParam);
			Sprite::SetTexture(m_pTexture[(int)TextureKind::PositionSeeat].get());
			Sprite::Draw();

			DirectX::XMFLOAT2 fNamePos = { -ce_fLineupNamePos.x + 40.0f,ce_fLineupNamePos.y + 9 * 48.0f };
			m_pWrite->DrawWString(itr->m_sName, fNamePos, D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);
			break;
		}
	}

	std::vector<CTeamManager::PitcherState> tVecPlayer2Pitcher = pPlayer2->GetPitcherState();
	i = 0;
	for (auto itr = tVecPlayer2Pitcher.begin(); itr != tVecPlayer2Pitcher.end(); itr++, i++)
	{
		if (itr->m_bEntry)
		{
			m_tVecPitcherSpriteParam[Player::Two][i].pos = { ce_fLineupPos.x,ce_fLineupPos.y - (ce_fLineupSize.y + 3.0f) * 9 };
			m_tVecPitcherSpriteParam[Player::Two][i].world = CCamera::Get2DWolrdMatrix(m_tVecPitcherSpriteParam[Player::Two][i].pos, m_tVecPitcherSpriteParam[Player::Two][i].rotate);
			Sprite::SetParam(m_tVecPitcherSpriteParam[Player::Two][i]);
			Sprite::SetTexture(m_pTexture[(int)TextureKind::PlayerSeeat].get());
			Sprite::Draw();

			m_tPositionSpriteParam.pos = { ce_fPositionPos.x,ce_fPositionPos.y - (ce_fPositionSize.y + 3.0f) * 9 };
			m_tPositionSpriteParam.uvPos.x = (float)((itr->m_eFieldingNo - 1) % ce_nSheetSplit) / (float)ce_nSheetSplit;
			m_tPositionSpriteParam.uvPos.y = (float)((itr->m_eFieldingNo - 1) / ce_nSheetSplit) / (float)ce_nSheetSplit;
			m_tPositionSpriteParam.world = CCamera::Get2DWolrdMatrix(m_tPositionSpriteParam.pos, m_tPositionSpriteParam.rotate);
			Sprite::SetParam(m_tPositionSpriteParam);
			Sprite::SetTexture(m_pTexture[(int)TextureKind::PositionSeeat].get());
			Sprite::Draw();


			DirectX::XMFLOAT2 fNamePos = { ce_fLineupNamePos.x,ce_fLineupNamePos.y + 9 * 48.0f };
			m_pWrite->DrawWString(itr->m_sName, fNamePos, D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);


			break;
		}
	}
}

void CSceneMemberselect::DrawBench()
{
	auto& spPlayer1 = CTeamManager::GetInstance(Player::One);
	CTeamManager* pPlayer1 = spPlayer1.get();
	auto& spPlayer2 = CTeamManager::GetInstance(Player::Two);
	CTeamManager* pPlayer2 = spPlayer2.get();

	
	m_tBenchPitcherSpriteParam.world = CCamera::Get2DWolrdMatrix(m_tBenchPitcherSpriteParam.pos, m_tBenchPitcherSpriteParam.rotate);
	Sprite::SetParam(m_tBenchPitcherSpriteParam);
	Sprite::SetTexture(m_pTexture[(int)TextureKind::BenchPitcher].get());
	Sprite::Draw();

	m_tBenchBatterSpriteParam.world = CCamera::Get2DWolrdMatrix(m_tBenchBatterSpriteParam.pos, m_tBenchBatterSpriteParam.rotate);
	Sprite::SetParam(m_tBenchBatterSpriteParam);
	Sprite::SetTexture(m_pTexture[(int)TextureKind::BenchBatter].get());
	Sprite::Draw();

	constexpr float ce_fBetween = 50.0f;
	std::vector<CTeamManager::PitcherState> tVecPitcherState;
	std::vector<CTeamManager::BatterState> tVecBatterState;
	int i = 0;
	switch (m_ePhase)
	{
	case CSceneMemberselect::SelectPhase::Player1Pitcher:
		i = 0;
		tVecPitcherState = pPlayer1->GetPitcherState();
		for (auto itr = tVecPitcherState.begin(); itr != tVecPitcherState.end(); itr++, i++)
		{
			if (itr->m_bStarter && !itr->m_bEntry)
			{
				m_tVecPitcherSpriteParam[Player::One][i].pos = { m_tBenchPitcherSpriteParam.pos.x + (itr->m_nStarterNo % 2 == 0 ? -150.0f : 160.0f) ,(m_tBenchPitcherSpriteParam.pos.y - ce_fBetween * (itr->m_nStarterNo / 2)) + 138.0f };
				m_tVecPitcherSpriteParam[Player::One][i].world = CCamera::Get2DWolrdMatrix(m_tVecPitcherSpriteParam[Player::One][i].pos, m_tVecPitcherSpriteParam[Player::One][i].rotate);
				Sprite::SetParam(m_tVecPitcherSpriteParam[Player::One][i]);
				Sprite::SetTexture(m_pTexture[(int)TextureKind::PlayerSeeat].get());
				Sprite::Draw();


				DirectX::XMFLOAT2 fNamePos = { m_tBenchPitcherSpriteParam.pos.x + (itr->m_nStarterNo % 2 == 0 ? -153.0f : 157.0f) ,(m_tBenchPitcherSpriteParam.pos.y + ce_fBetween * (itr->m_nStarterNo / 2)) - 163.0f };
				m_pWrite->DrawWString(itr->m_sName, fNamePos, D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);
			}
			else if (itr->m_nBenchNo != 0)
			{
				m_tVecPitcherSpriteParam[Player::One][i].pos = { m_tBenchPitcherSpriteParam.pos.x + ((itr->m_nBenchNo - 1) % 2 == 0 ? -150.0f : 160.0f) ,(m_tBenchPitcherSpriteParam.pos.y - ce_fBetween * ((itr->m_nBenchNo - 1) / 2)) - 80.0f };
				m_tVecPitcherSpriteParam[Player::One][i].world = CCamera::Get2DWolrdMatrix(m_tVecPitcherSpriteParam[Player::One][i].pos, m_tVecPitcherSpriteParam[Player::One][i].rotate);
				Sprite::SetParam(m_tVecPitcherSpriteParam[Player::One][i]);
				Sprite::SetTexture(m_pTexture[(int)TextureKind::PlayerSeeat].get());
				Sprite::Draw();

				DirectX::XMFLOAT2 fNamePos = { m_tBenchPitcherSpriteParam.pos.x + ((itr->m_nBenchNo - 1) % 2 == 0 ? -153.0f : 157.0f) ,(m_tBenchPitcherSpriteParam.pos.y + ce_fBetween * ((itr->m_nBenchNo - 1) / 2)) + 55.0f };
				m_pWrite->DrawWString(itr->m_sName, fNamePos, D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);

			}
		}
		break;
	case CSceneMemberselect::SelectPhase::Player1Batter:
		i = 0;
		tVecBatterState = pPlayer1->GetBatterState();
		for (auto itr = tVecBatterState.begin(); itr != tVecBatterState.end(); itr++, i++)
		{
			if (itr->m_nBenchNo != 0)
			{
				m_tVecBatterSpriteParam[Player::One][i].pos = { m_tBenchBatterSpriteParam.pos.x + ((itr->m_nBenchNo - 1) % 2 == 0 ? -150.0f : 160.0f) ,(m_tBenchBatterSpriteParam.pos.y - ce_fBetween * ((itr->m_nBenchNo - 1) / 2)) + 138.0f };
				m_tVecBatterSpriteParam[Player::One][i].world = CCamera::Get2DWolrdMatrix(m_tVecBatterSpriteParam[Player::One][i].pos, m_tVecBatterSpriteParam[Player::One][i].rotate);
				Sprite::SetParam(m_tVecBatterSpriteParam[Player::One][i]);
				Sprite::SetTexture(m_pTexture[(int)TextureKind::PlayerSeeat].get());
				Sprite::Draw();


				DirectX::XMFLOAT2 fNamePos = { m_tBenchBatterSpriteParam.pos.x + ((itr->m_nBenchNo - 1) % 2 == 0 ? -153.0f : 157.0f) ,(m_tBenchBatterSpriteParam.pos.y + ce_fBetween * ((itr->m_nBenchNo - 1) / 2)) - 163.0f };
				m_pWrite->DrawWString(itr->m_sName, fNamePos, D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);

			}
		}
		break;
	case CSceneMemberselect::SelectPhase::Player2Pitcher:
		i = 0;
		tVecPitcherState = pPlayer2->GetPitcherState();
		for (auto itr = tVecPitcherState.begin(); itr != tVecPitcherState.end(); itr++, i++)
		{
			if (itr->m_bStarter && !itr->m_bEntry)
			{
				m_tVecPitcherSpriteParam[Player::Two][i].pos = { m_tBenchPitcherSpriteParam.pos.x + (itr->m_nStarterNo % 2 == 0 ? -150.0f : 160.0f) ,(m_tBenchPitcherSpriteParam.pos.y - ce_fBetween * (itr->m_nStarterNo / 2)) + 138.0f };
				m_tVecPitcherSpriteParam[Player::Two][i].world = CCamera::Get2DWolrdMatrix(m_tVecPitcherSpriteParam[Player::Two][i].pos, m_tVecPitcherSpriteParam[Player::Two][i].rotate);
				Sprite::SetParam(m_tVecPitcherSpriteParam[Player::Two][i]);
				Sprite::SetTexture(m_pTexture[(int)TextureKind::PlayerSeeat].get());
				Sprite::Draw();


				DirectX::XMFLOAT2 fNamePos = { m_tBenchPitcherSpriteParam.pos.x + (itr->m_nStarterNo % 2 == 0 ? -153.0f : 157.0f) ,(m_tBenchPitcherSpriteParam.pos.y + ce_fBetween * (itr->m_nStarterNo / 2)) - 163.0f };
				m_pWrite->DrawWString(itr->m_sName, fNamePos, D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);

			}
			else if (itr->m_nBenchNo != 0)
			{
				m_tVecPitcherSpriteParam[Player::Two][i].pos = { m_tBenchPitcherSpriteParam.pos.x + ((itr->m_nBenchNo - 1) % 2 == 0 ? -150.0f : 160.0f) ,(m_tBenchPitcherSpriteParam.pos.y - ce_fBetween * ((itr->m_nBenchNo - 1) / 2)) - 80.0f };
				m_tVecPitcherSpriteParam[Player::Two][i].world = CCamera::Get2DWolrdMatrix(m_tVecPitcherSpriteParam[Player::Two][i].pos, m_tVecPitcherSpriteParam[Player::Two][i].rotate);
				Sprite::SetParam(m_tVecPitcherSpriteParam[Player::Two][i]);
				Sprite::SetTexture(m_pTexture[(int)TextureKind::PlayerSeeat].get());
				Sprite::Draw();

				DirectX::XMFLOAT2 fNamePos = { m_tBenchPitcherSpriteParam.pos.x + ((itr->m_nBenchNo - 1) % 2 == 0 ? -153.0f : 157.0f) ,(m_tBenchPitcherSpriteParam.pos.y + ce_fBetween * ((itr->m_nBenchNo - 1) / 2)) + 55.0f };
				m_pWrite->DrawWString(itr->m_sName, fNamePos, D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);

			}
		}
		break;
	case CSceneMemberselect::SelectPhase::Player2Batter:
		i = 0;
		tVecBatterState = pPlayer2->GetBatterState();
		for (auto itr = tVecBatterState.begin(); itr != tVecBatterState.end(); itr++, i++)
		{
			if (itr->m_nBenchNo != 0)
			{
				m_tVecBatterSpriteParam[Player::Two][i].pos = { m_tBenchBatterSpriteParam.pos.x + ((itr->m_nBenchNo - 1) % 2 == 0 ? -150.0f : 160.0f) ,(m_tBenchBatterSpriteParam.pos.y - ce_fBetween * ((itr->m_nBenchNo - 1) / 2)) + 138.0f };
				m_tVecBatterSpriteParam[Player::Two][i].world = CCamera::Get2DWolrdMatrix(m_tVecBatterSpriteParam[Player::Two][i].pos, m_tVecBatterSpriteParam[Player::Two][i].rotate);
				Sprite::SetParam(m_tVecBatterSpriteParam[Player::Two][i]);
				Sprite::SetTexture(m_pTexture[(int)TextureKind::PlayerSeeat].get());
				Sprite::Draw();


				DirectX::XMFLOAT2 fNamePos = { m_tBenchBatterSpriteParam.pos.x + ((itr->m_nBenchNo - 1) % 2 == 0 ? -153.0f : 157.0f) ,(m_tBenchBatterSpriteParam.pos.y + ce_fBetween * ((itr->m_nBenchNo - 1) / 2)) - 163.0f };
				m_pWrite->DrawWString(itr->m_sName, fNamePos, D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);

			}
		}
		break;
	default:
		break;
	}




}

void CSceneMemberselect::DrawCursor()
{
	static DirectX::XMFLOAT2 pos = {};
	static DirectX::XMFLOAT2 size = {};
	SpriteParamDebug(&pos, &size, "Name");

//	m_tCursorParam[Player::One].pos = pos;

	switch (m_ePhase)
	{
	case CSceneMemberselect::SelectPhase::Home:
		// カーソルの描画
		for (int i = 0; i < Player::MaxPlayer; i++)
		{
			m_tCursorParam[i].pos.x = -80.0f + 160.0f * i;
			m_tCursorParam[i].pos.y = 248.0f - ce_fCursorMove * m_nCursor[i];
			m_tCursorParam[i].world = CCamera::Get2DWolrdMatrix(m_tCursorParam[i].pos, m_tCursorParam[i].rotate);
			Sprite::SetParam(m_tCursorParam[i]);
			Sprite::SetTexture(m_pTexture[(int)TextureKind::Cursor].get());
			Sprite::Draw();
		}
		break;
	case CSceneMemberselect::SelectPhase::Player1Pitcher:
	case CSceneMemberselect::SelectPhase::Player1Batter:
		m_tCursorParam[Player::One].world = CCamera::Get2DWolrdMatrix(m_tCursorParam[Player::One].pos, m_tCursorParam[Player::One].rotate);
		Sprite::SetParam(m_tCursorParam[Player::One]);
		Sprite::SetTexture(m_pTexture[(int)TextureKind::Cursor].get());
		Sprite::Draw();
		break;
	case CSceneMemberselect::SelectPhase::Player1Option:
		break;
	case CSceneMemberselect::SelectPhase::Player2Pitcher:
	case CSceneMemberselect::SelectPhase::Player2Batter:
		m_tCursorParam[Player::Two].world = CCamera::Get2DWolrdMatrix(m_tCursorParam[Player::Two].pos, m_tCursorParam[Player::Two].rotate);
		Sprite::SetParam(m_tCursorParam[Player::Two]);
		Sprite::SetTexture(m_pTexture[(int)TextureKind::Cursor].get());
		Sprite::Draw();
		break;
	case CSceneMemberselect::SelectPhase::Player2Option:
		break;
	case CSceneMemberselect::SelectPhase::GameOption:
		break;
	default:
		break;
	}
}
*/
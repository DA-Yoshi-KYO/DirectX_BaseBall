#include "SceneMemberselect.h"
#include "Main.h"
#include "Camera.h"
#include "Sprite.h"
#include "ImGuiManager.h"
#include "Input.h"

constexpr float ce_fCursorMove = 75.0f;
constexpr DirectX::XMFLOAT2 ce_fLineupPos = { 375.0f,170.0f };
constexpr DirectX::XMFLOAT2 ce_fLineupSize = { 240.0f,45.0f };

CSceneMemberselect::CSceneMemberselect(CTeamManager::Teams player1, CTeamManager::Teams player2)
	: m_pTexture{}
	, m_tVecPitcherSpriteParam{}, m_tVecBatterSpriteParam{}
	, m_ePhase(SelectPhase::Home), m_nCursor{ (int)CursorPhase::Start,(int)CursorPhase::Start }
	, m_tBackParam{}, m_tCursorParam{}
	, m_bEnd(false), m_bReady{false,false}
{
	CTeamManager* pPlayer1 = CTeamManager::GetInstance(Player::One).get();
	CTeamManager* pPlayer2 = CTeamManager::GetInstance(Player::Two).get();

	int nStarter = rand() % 6;

	if(!pPlayer1->Load(player1)) SetEnd(true);
	if(!pPlayer2->Load(player2)) SetEnd(true);

	SpriteParam tParam;
	DirectX::XMFLOAT4X4 vp[2];
	vp[0] = CCamera::Get2DViewMatrix();
	vp[1] = CCamera::Get2DProjectionMatrix();

	m_pWrite = std::make_unique<CWrite>(&m_tFont);
	m_pWrite->Init(GetSwapChain());

	// フォントデータを改変
	m_tFont.m_fFontSize = 80;
	m_tFont.m_eFontThick = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	m_tFont.m_Color = D2D1::ColorF(D2D1::ColorF::Black);
	m_tFont.m_wsFont = m_pWrite->GetFontName(0);
	m_tFont.m_ShadowColor = D2D1::ColorF(D2D1::ColorF::White);
	m_tFont.m_ShadowOffset = D2D1::Point2F(5.0f, -5.0f);
	m_tFont.m_eTextAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;

	// フォントをセット
	m_pWrite->SetFont(m_tFont);

	std::vector<CTeamManager::PitcherState> tVecPlayer1Pitcher = pPlayer1->GetPitcherState();
	for (auto itr = tVecPlayer1Pitcher.begin(); itr != tVecPlayer1Pitcher.end(); itr++)
	{
		tParam.pos = { 0.0f,0.0f };
		tParam.size = ce_fLineupSize;
		tParam.rotate = 0.0f;
		tParam.offsetPos = { 0.0f,0.0f };
		tParam.color = { 1.0f,1.0f,1.0f,1.0f };
		tParam.uvPos = { 0.0f / (float)ce_nSheetSplit,3.0f / (float)ce_nSheetSplit };
		tParam.uvSize = { 3.0f / (float)ce_nSheetSplit,1.0f / (float)ce_nSheetSplit };
		tParam.view = vp[0];
		tParam.proj = vp[1];
		m_tVecPitcherSpriteParam[Player::One].push_back(tParam);
	}
	std::vector<CTeamManager::BatterState> tVecPlayer1Batter = pPlayer1->GetBatterState();
	for (auto itr = tVecPlayer1Batter.begin(); itr != tVecPlayer1Batter.end(); itr++)
	{
		tParam.pos = { 0.0f,0.0f };
		tParam.size = ce_fLineupSize;
		tParam.rotate = 0.0f;
		tParam.offsetPos = { 0.0f,0.0f };
		tParam.color = { 1.0f,1.0f,1.0f,1.0f };

		switch ((*itr).m_eAptitude)
		{
		case CTeamManager::FieldingNo::Chatcher:
			tParam.uvPos = { 0.0f / (float)ce_nSheetSplit, 2.0f / (float)ce_nSheetSplit };
			break;
		case CTeamManager::FieldingNo::First:
		case CTeamManager::FieldingNo::Second:
		case CTeamManager::FieldingNo::Third:
		case CTeamManager::FieldingNo::Short:
			tParam.uvPos = { 0.0f / (float)ce_nSheetSplit, 1.0f / (float)ce_nSheetSplit };
			break;
		case CTeamManager::FieldingNo::Left:
		case CTeamManager::FieldingNo::Center:
		case CTeamManager::FieldingNo::Right:
			tParam.uvPos = { 0.0f / (float)ce_nSheetSplit,0.0f / (float)ce_nSheetSplit };
			break;
		default:
			break;
		}

		tParam.uvSize = { 3.0f / (float)ce_nSheetSplit,1.0f / (float)ce_nSheetSplit };
		tParam.view = vp[0];
		tParam.proj = vp[1];
		m_tVecBatterSpriteParam[Player::One].push_back(tParam);
	}
	std::vector<CTeamManager::PitcherState> tVecPlayer2Pitcher = pPlayer2->GetPitcherState();
	for (auto itr = tVecPlayer2Pitcher.begin(); itr != tVecPlayer2Pitcher.end(); itr++)
	{
		tParam.pos = { 0.0f,0.0f };
		tParam.size = ce_fLineupSize;
		tParam.rotate = 0.0f;
		tParam.offsetPos = { 0.0f,0.0f };
		tParam.color = { 1.0f,1.0f,1.0f,1.0f };
		tParam.uvPos = { 0.0f / (float)ce_nSheetSplit,3.0f / (float)ce_nSheetSplit };
		tParam.uvSize = { 3.0f / (float)ce_nSheetSplit,1.0f / (float)ce_nSheetSplit };
		tParam.view = vp[0];
		tParam.proj = vp[1];
		m_tVecPitcherSpriteParam[Player::Two].push_back(tParam);
	}
	std::vector<CTeamManager::BatterState> tVecPlayer2Batter = pPlayer2->GetBatterState();
	for (auto itr = tVecPlayer2Batter.begin(); itr != tVecPlayer2Batter.end(); itr++)
	{
		tParam.pos = { 0.0f,0.0f };
		tParam.size = ce_fLineupSize;
		tParam.rotate = 0.0f;
		tParam.offsetPos = { 0.0f,0.0f };
		tParam.color = { 1.0f,1.0f,1.0f,1.0f };

		switch ((*itr).m_eAptitude)
		{
		case CTeamManager::FieldingNo::Chatcher:
			tParam.uvPos = { 0.0f / (float)ce_nSheetSplit, 2.0f / (float)ce_nSheetSplit };
			break;
		case CTeamManager::FieldingNo::First:
		case CTeamManager::FieldingNo::Second:
		case CTeamManager::FieldingNo::Third:
		case CTeamManager::FieldingNo::Short:
			tParam.uvPos = { 0.0f / (float)ce_nSheetSplit, 1.0f / (float)ce_nSheetSplit };
			break;
		case CTeamManager::FieldingNo::Left:
		case CTeamManager::FieldingNo::Center:
		case CTeamManager::FieldingNo::Right:
			tParam.uvPos = { 0.0f / (float)ce_nSheetSplit,0.0f / (float)ce_nSheetSplit };
			break;
		default:
			break;
		}

		tParam.uvSize = { 3.0f / (float)ce_nSheetSplit,1.0f / (float)ce_nSheetSplit };
		tParam.view = vp[0];
		tParam.proj = vp[1];
		m_tVecBatterSpriteParam[Player::Two].push_back(tParam);
	}
	while (1)
	{
		int nRand = rand() % tVecPlayer1Pitcher.size();
		if (tVecPlayer1Pitcher[nRand].m_bStarter)
		{
			pPlayer1->SetEntry(nRand, true, true);
			break;
		}
	}
	while (1)
	{
		int nRand = rand() % tVecPlayer2Pitcher.size();
		if (tVecPlayer2Pitcher[nRand].m_bStarter)
		{
			pPlayer2->SetEntry(nRand, true, true);
			break;
		}
	}
	
	m_tBackParam.pos = { 0.0f,0.0f };
	m_tBackParam.size = { SCREEN_WIDTH,SCREEN_HEIGHT };
	m_tBackParam.rotate = 0.0f;
	m_tBackParam.color = { 1.0f,1.0f,1.0f,1.0f };
	m_tBackParam.offsetPos = { 0.0f,0.0f };
	m_tBackParam.uvPos = { 0.0f,0.0f };
	m_tBackParam.uvSize = { 1.0f,1.0f };
	m_tBackParam.view = vp[0];
	m_tBackParam.proj = vp[1];

	m_tCursorParam[Player::One].pos = { -80.0f,230.0f };
	m_tCursorParam[Player::One].size = { 40.0f,40.0f };
	m_tCursorParam[Player::One].rotate = 0.0f;
	m_tCursorParam[Player::One].color = { 1.0f,0.0f,0.0f,1.0f };
	m_tCursorParam[Player::One].offsetPos = { 0.0f,0.0f };
	m_tCursorParam[Player::One].uvPos = { 0.0f,0.0f };
	m_tCursorParam[Player::One].uvSize = { 1.0f,1.0f };
	m_tCursorParam[Player::One].view = vp[0];
	m_tCursorParam[Player::One].proj = vp[1];

	m_tCursorParam[Player::Two].pos = { 80.0f,230.0f };
	m_tCursorParam[Player::Two].size = { 40.0f,40.0f };
	m_tCursorParam[Player::Two].rotate = 0.0f;
	m_tCursorParam[Player::Two].color = { 0.0f,1.0f,0.0f,1.0f };
	m_tCursorParam[Player::Two].offsetPos = { 0.0f,0.0f };
	m_tCursorParam[Player::Two].uvPos = { 0.0f,0.0f };
	m_tCursorParam[Player::Two].uvSize = { 1.0f,1.0f };
	m_tCursorParam[Player::Two].view = vp[0];
	m_tCursorParam[Player::Two].proj = vp[1];

	for (int i = 0; i < (int)TextureKind::Max; i++)
	{
		m_pTexture[i] = std::make_unique<Texture>();
	}
	if(FAILED(m_pTexture[(int)TextureKind::Back]->Create(PATH_TEX("MemberSelectBack.jpg")))) ERROR_MESSAGE("MemberSelectBack.jpg");
	if(FAILED(m_pTexture[(int)TextureKind::PlayerSeeat]->Create(PATH_TEX("PlayerSeeat.png")))) ERROR_MESSAGE("PlayerSeeat.png");
	if(FAILED(m_pTexture[(int)TextureKind::Cursor]->Create(PATH_TEX("Ball.png")))) ERROR_MESSAGE("Ball.png");
}

CSceneMemberselect::~CSceneMemberselect()
{

}

void CSceneMemberselect::Update()
{
	static float fTime = 0.0f;

	if (!m_bEnd)
	{
		switch (m_ePhase)
		{
		case CSceneMemberselect::SelectPhase::Home:

			m_tCursorParam[Player::One].rotate = DirectX::XMConvertToRadians(fTime * 180.0f);
			m_tCursorParam[Player::Two].rotate = DirectX::XMConvertToRadians(fTime * 180.0f);
			fTime += 1.0f / fFPS;

			for (int i = 0; i < Player::MaxPlayer; i++)
			{
				UpdateHome((Player)i);
				if (m_ePhase != SelectPhase::Home) break;
			}

			break;
		case CSceneMemberselect::SelectPhase::Player1Pitcher:
			break;
		case CSceneMemberselect::SelectPhase::Player1Batter:
			break;
		case CSceneMemberselect::SelectPhase::Player1Option:
			break;
		case CSceneMemberselect::SelectPhase::Player2Pitcher:
			break;
		case CSceneMemberselect::SelectPhase::Player2Batter:
			break;
		case CSceneMemberselect::SelectPhase::Player2Option:
			break;
		case CSceneMemberselect::SelectPhase::GameOption:
			break;
		default:
			break;
		}

		if (m_bReady[Player::One]  && m_bReady[Player::Two])
		{
			SetNext(SceneKind::TeamSelect);
			m_bEnd = true;
		}
	}
}

void CSceneMemberselect::Draw()
{
	CTeamManager* pPlayer1 = CTeamManager::GetInstance(Player::One).get();
	CTeamManager* pPlayer2 = CTeamManager::GetInstance(Player::Two).get();

	// 背景の描画
	m_tBackParam.world = CCamera::Get2DWolrdMatrix(m_tBackParam.pos, m_tBackParam.rotate);
	Sprite::SetParam(m_tBackParam);
	Sprite::SetTexture(m_pTexture[(int)TextureKind::Back].get());
	Sprite::Draw();

	// カーソルの描画
	for (int i = 0; i < Player::MaxPlayer; i++)
	{
		m_tCursorParam[i].pos.y = 230.0f - ce_fCursorMove * m_nCursor[i];
		m_tCursorParam[i].world = CCamera::Get2DWolrdMatrix(m_tCursorParam[i].pos, m_tCursorParam[i].rotate);
		Sprite::SetParam(m_tCursorParam[i]);
		Sprite::SetTexture(m_pTexture[(int)TextureKind::Cursor].get());
		Sprite::Draw();
	}

	// 選択肢文字の描画
	DirectX::XMFLOAT2 fStringPos[(int)CursorPhase::Max] = {
		{0.0f,-253.0f + ce_fCursorMove * (int)CursorPhase::Start},
		{0.0f,-253.0f + ce_fCursorMove * (int)CursorPhase::PitcherChange},
		{0.0f,-253.0f + ce_fCursorMove * (int)CursorPhase::BatterChange},
		{0.0f,-253.0f + ce_fCursorMove * (int)CursorPhase::UserOption},
		{0.0f,-253.0f + ce_fCursorMove * (int)CursorPhase::GameOption},
	};
	m_tFont.m_fFontSize = 30.0f;
	m_pWrite->SetFont(m_tFont);
	m_pWrite->DrawString("試合開始", fStringPos[(int)CursorPhase::Start], D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);
	m_pWrite->DrawString("投手変更", fStringPos[(int)CursorPhase::PitcherChange], D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);
	m_pWrite->DrawString("野手変更", fStringPos[(int)CursorPhase::BatterChange], D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);
	m_pWrite->DrawString("操作設定", fStringPos[(int)CursorPhase::UserOption], D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);
	m_pWrite->DrawString("試合設定", fStringPos[(int)CursorPhase::GameOption], D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);

	// スタメン選手のポジションの描画
	
	std::vector<CTeamManager::BatterState> tVecPlayer1Batter = pPlayer1->GetBatterState();
	int i = 0;
	for (auto itr = tVecPlayer1Batter.begin(); itr != tVecPlayer1Batter.end(); itr++,i++)
	{
		if (itr->m_nLineupNo == 0) continue;
		
		m_tVecBatterSpriteParam[Player::One][i].pos = { -ce_fLineupPos.x + 65.0f,ce_fLineupPos.y - (ce_fLineupSize.y + 3.0f) * (itr->m_nLineupNo - 1) };
		m_tVecBatterSpriteParam[Player::One][i].world = CCamera::Get2DWolrdMatrix(m_tVecBatterSpriteParam[Player::One][i].pos, m_tVecBatterSpriteParam[Player::One][i].rotate);
		Sprite::SetParam(m_tVecBatterSpriteParam[Player::One][i]);
		Sprite::SetTexture(m_pTexture[(int)TextureKind::PlayerSeeat].get());
		Sprite::Draw();
	}

	std::vector<CTeamManager::BatterState> tVecPlayer2Batter = pPlayer2->GetBatterState();
	i = 0;
	for (auto itr = tVecPlayer2Batter.begin(); itr != tVecPlayer2Batter.end(); itr++,i++)
	{
		if (itr->m_nLineupNo == 0) continue;

		m_tVecBatterSpriteParam[Player::Two][i].pos = { ce_fLineupPos.x,ce_fLineupPos.y - (ce_fLineupSize.y + 3.0f) * (itr->m_nLineupNo - 1) };
		m_tVecBatterSpriteParam[Player::Two][i].world = CCamera::Get2DWolrdMatrix(m_tVecBatterSpriteParam[Player::Two][i].pos, m_tVecBatterSpriteParam[Player::Two][i].rotate);
		Sprite::SetParam(m_tVecBatterSpriteParam[Player::Two][i]);
		Sprite::SetTexture(m_pTexture[(int)TextureKind::PlayerSeeat].get());
		Sprite::Draw();

	}

	std::vector<CTeamManager::PitcherState> tVecPlayer1Pitcher = pPlayer1->GetPitcherState();
	i = 0;
	for (auto itr = tVecPlayer1Pitcher.begin(); itr != tVecPlayer1Pitcher.end(); itr++,i++)
	{
		if (itr->m_bEntry)
		{
			m_tVecPitcherSpriteParam[Player::One][i].pos = { -ce_fLineupPos.x + 65.0f,ce_fLineupPos.y - (ce_fLineupSize.y + 3.0f) * 9 };
			m_tVecPitcherSpriteParam[Player::One][i].world = CCamera::Get2DWolrdMatrix(m_tVecPitcherSpriteParam[Player::One][i].pos, m_tVecPitcherSpriteParam[Player::One][i].rotate);
			Sprite::SetParam(m_tVecPitcherSpriteParam[Player::One][i]);
			Sprite::SetTexture(m_pTexture[(int)TextureKind::PlayerSeeat].get());
			Sprite::Draw();
			break;
		}
	}
	
	std::vector<CTeamManager::PitcherState> tVecPlayer2Pitcher = pPlayer2->GetPitcherState();
	i = 0;
	for (auto itr = tVecPlayer2Pitcher.begin(); itr != tVecPlayer2Pitcher.end(); itr++,i++)
	{
		if (itr->m_bEntry)
		{
			m_tVecPitcherSpriteParam[Player::Two][i].pos = { ce_fLineupPos.x,ce_fLineupPos.y - (ce_fLineupSize.y + 3.0f) * 9 };
			m_tVecPitcherSpriteParam[Player::Two][i].world = CCamera::Get2DWolrdMatrix(m_tVecPitcherSpriteParam[Player::Two][i].pos, m_tVecPitcherSpriteParam[Player::Two][i].rotate);
			Sprite::SetParam(m_tVecPitcherSpriteParam[Player::Two][i]);
			Sprite::SetTexture(m_pTexture[(int)TextureKind::PlayerSeeat].get());
			Sprite::Draw();
			break;
		}
	}
	
}

void CSceneMemberselect::UpdateHome(Player player)
{
	if (player == Player::MaxPlayer)
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
				switch (player)
				{
				case One: m_ePhase = SelectPhase::Player1Pitcher; break;
				case Two: m_ePhase = SelectPhase::Player2Pitcher; break;
				}
				break;
			case (int)CSceneMemberselect::CursorPhase::BatterChange:
				switch (player)
				{
				case One: m_ePhase = SelectPhase::Player1Batter; break;
				case Two: m_ePhase = SelectPhase::Player2Batter; break;
				}
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

#include "MemberSelectDirector.h"
#include "Main.h"
#include "MemberSelectBackGround.h"
#include "PitcherData.h"
#include "FielderData.h"
#include "Input.h"
#include "SceneGame.h"

enum class SelectKind
{
	Start,
	PitcherSelect,
	BatterSelect,
	Max
};

CMemberSelectDirector::CMemberSelectDirector()
	: m_nSelectIndex(), m_bReady{}, m_pCursor{}
	, m_pBenchFielder{}, m_pBenchPitcher{}, m_pStartingLineup{}
	, m_pPositionLineup{} , m_pPitcherIcon{}
	, m_pTeams{}
	, m_bEnd(false)
{

}

CMemberSelectDirector::~CMemberSelectDirector()
{

}

void CMemberSelectDirector::Init(TeamKind kind1, TeamKind kind2)
{
	CScene* pScene = GetScene();	// オブジェクト追加用に現在シーンを取得

	// ---背景の追加
	pScene->AddGameObject<CMemberSelectBackGround>("BackGround", Tag::UI);

	// ---プレイヤー1の初期化処理
	// チームデータの作成・読み込み
	m_pTeams[0] = std::make_unique<CTeamDirector>(1);
	CTeam* pTeam1 = m_pTeams[0]->GetTeam();
	pTeam1->Load(kind1);
	m_pTeams[0]->TeamInit();
	
	// ---選手アイコンの作成
	// 各種アイコンデータクラスのインスタンスを生成
	for (int i = 0; i < m_pBenchFielder.size(); i++)
	{
		m_pBenchFielder[i] = std::make_unique<CSelectFielderField>();
	}
	for (int i = 0; i < m_pBenchPitcher.size(); i++)
	{
		m_pBenchPitcher[i] = std::make_unique<CSelectPitcherField>();
	}
	for (int i = 0; i < m_pStartingLineup.size(); i++)
	{
		m_pStartingLineup[i] = std::make_unique<CStartingLineupField>();
	}
	
	// コピー用変数の定義
	std::list<CFielderData*> pAllFielder = pTeam1->GetFielderMember();	// 全野手リスト
	std::list<CPitcherData*> pAllPitcher = pTeam1->GetPitcherMember();	// 全投手リスト
	std::list<CFielderData*> pStartingFielder = pTeam1->GetStartingLineup();	// 初期スタメン野手リスト
	std::list<CFielderData*> pBenchFielder;		// 初期ベンチ野手リスト
	CPitcherData* pStarterPitcher = pTeam1->GetStarterPitcher(); // 初期先発投手データ
	std::list<CPitcherData*> pBenchPitcher;		// 初期ベンチ投手リスト
	
	// 野手データの振り分け
	for (auto itr : pAllFielder)
	{
		// データからベンチ野手を振り分け
		if (itr->GetPlayerData().m_nLineupNo == 0) pBenchFielder.push_back(itr);
	}
	
	// 投手データの振り分け
	for (auto itr : pAllPitcher)
	{
		// ベンチ投手を振り分け
		if (pStarterPitcher != itr) pBenchPitcher.push_back(itr);
	}

	// それぞれのデータを元に選手用アイコンを作成
	m_pStartingLineup[0]->Init(1, pStartingFielder, pStarterPitcher);
	m_pBenchFielder[0]->Init(1, pBenchFielder);
	m_pBenchPitcher[0]->Init(1, pBenchPitcher);

	// ポジション用アイコンを作成
	// 野手ポジションアイコンをスタメンに沿って作成
	int index = 0;
	for (auto itr : pStartingFielder)
	{
		m_pPositionLineup[0][index] = pScene->AddGameObject<CPositionIcon>("PositionLineupIcon", Tag::UI);
		m_pPositionLineup[0][index]->Init(itr->GetPlayerData().m_eEntryPosition);
		m_pPositionLineup[0][index]->SetPos(DirectX::XMFLOAT3(165.0f, 205.0f + 47.0f * index, 0.0f));
		m_pPositionLineup[0][index]->SetSize(DirectX::XMFLOAT3(45.0f, 45.0f, 0.0f));

		++index;
	}
	// 投手アイコンは変更がない為位置を固定して作成
	m_pPitcherIcon[0] = pScene->AddGameObject<CPositionIcon>("PitcherIcon", Tag::UI);
	m_pPitcherIcon[0]->Init(Positions::Pitcher);
	m_pPitcherIcon[0]->SetPos(DirectX::XMFLOAT3(163.0f, 628.0f, 0.0f));
	m_pPitcherIcon[0]->SetSize(DirectX::XMFLOAT3(45.0f, 45.0f, 0.0f));

	// ---プレイヤー2の初期化処理
	// チームデータの作成・読み込み
	m_pTeams[1] = std::make_unique<CTeamDirector>(2);
	CTeam* pTeam2 = m_pTeams[1]->GetTeam();
	pTeam2->Load(kind2);
	m_pTeams[1]->TeamInit();

	// 選手アイコンの作成
	// 全野手、投手リストと先発投手のデータをプレイヤー2のものに更新
	pAllFielder = pTeam2->GetFielderMember();	
	pAllPitcher = pTeam2->GetPitcherMember();
	pStarterPitcher = pTeam2->GetStarterPitcher();
	pStartingFielder = pTeam2->GetStartingLineup();
	// プレイヤー2用の振り分けを行うのでリストをクリア
	pBenchFielder.clear();
	pBenchPitcher.clear();

	// 野手データの振り分け
	for (auto itr : pAllFielder)
	{
		// データからベンチ野手を振り分け
		if (itr->GetPlayerData().m_nLineupNo == 0) pBenchFielder.push_back(itr);
	}
	
	// 投手データの振り分け
	for (auto itr : pAllPitcher)
	{
		// ベンチ投手を振り分け
		if (pStarterPitcher != itr) pBenchPitcher.push_back(itr);
	}

	// それぞれのデータを元に選手用アイコンを作成
	m_pStartingLineup[1]->Init(2, pStartingFielder, pStarterPitcher);
	m_pBenchFielder[1]->Init(2, pBenchFielder);
	m_pBenchPitcher[1]->Init(2, pBenchPitcher);

	// ポジション用アイコンを作成
	// 野手ポジションアイコンをスタメンに沿って作成
	index = 0;
	for (auto itr : pStartingFielder)
	{
		m_pPositionLineup[1][index] = pScene->AddGameObject<CPositionIcon>("PositionLineupIcon", Tag::UI);
		m_pPositionLineup[1][index]->Init(itr->GetPlayerData().m_eEntryPosition);
		m_pPositionLineup[1][index]->SetPos(DirectX::XMFLOAT3(930.0f, 205.0f + 47.0f * index, 0.0f));
		m_pPositionLineup[1][index]->SetSize(DirectX::XMFLOAT3(45.0f, 45.0f, 0.0f));

		++index;
	}
	// 投手アイコンは変更がない為位置を固定して作成
	m_pPitcherIcon[1] = pScene->AddGameObject<CPositionIcon>("PitcherIcon", Tag::UI);
	m_pPitcherIcon[1]->Init(Positions::Pitcher);
	m_pPitcherIcon[1]->SetPos(DirectX::XMFLOAT3(930.0f, 628.0f, 0.0f));
	m_pPitcherIcon[1]->SetSize(DirectX::XMFLOAT3(45.0f, 45.0f, 0.0f));

	//---カーソルの作成
	for (int i = 0; i < m_pCursor.size(); i++)
	{
		m_pCursor[i] = pScene->AddGameObject<CMemberSelectCursor>("Cursor", Tag::UI);
	}
	m_pCursor[0]->SetColor(DirectX::XMFLOAT4(0, 1, 0, 1));
	m_pCursor[0]->SetPos(DirectX::XMFLOAT3(SCREEN_WIDTH * 0.5f - 114.0f, 114.0f, 0.0f));
	m_pCursor[1]->SetColor(DirectX::XMFLOAT4(1, 0, 0, 1));
	m_pCursor[1]->SetPos(DirectX::XMFLOAT3(SCREEN_WIDTH * 0.5f + 114.0f, 114.0f, 0.0f));
}

void CMemberSelectDirector::Update()
{
	if (m_bEnd) return;
	Input();
	for (int i = 0; i < m_pBenchFielder.size(); i++)
	{
		if (m_bReady[i]) continue;
		m_pBenchFielder[i]->Update();
	}
	for (int i = 0; i < m_pBenchPitcher.size(); i++)
	{
		if (m_bReady[i]) continue;
		m_pBenchPitcher[i]->Update();
	}
	for (int i = 0; i < m_pStartingLineup.size(); i++)
	{
		if (m_bReady[i]) continue;
		m_pStartingLineup[i]->Update();
	}

	if (m_bReady[0] && m_bReady[1])
	{
		if (!m_bEnd)
		{
			m_bEnd = true;
			FadeOut([]()
				{
					ChangeScene(new CSceneGame());
					FadeIn(nullptr);
				});
		}
	}
}

void CMemberSelectDirector::Input()
{
	for (int i = 0; i < 2; i++)
	{
		if (m_pBenchPitcher[i]->GetActive() || m_pBenchFielder[i]->GetActive())
			break;
		if (m_bReady[i]) continue;

		if (IsKeyTrigger(i + 1, Input::Down))
		{
			if (m_nSelectIndex[i] != int(SelectKind::Max) - 1) m_nSelectIndex[i]++;
		}
		else if (IsKeyTrigger(i + 1, Input::Up))
		{
			if (m_nSelectIndex[i] != int(SelectKind::Start)) m_nSelectIndex[i]--;
		}
		else if (IsKeyTrigger(i + 1, Input::A))
		{
			switch (m_nSelectIndex[i])
			{
				case int(SelectKind::Start) :
					m_bReady[i] = true;
					break;
				case int(SelectKind::PitcherSelect) :
					m_pBenchPitcher[i]->SetActive(true);
					break;
				case int(SelectKind::BatterSelect) :
					m_pBenchFielder[i]->SetActive(true);
					break;
				default:
					break;
			}
		}
		m_pCursor[i]->SetPos(DirectX::XMFLOAT3((SCREEN_WIDTH * 0.5f - 114.0f) + (114.0f * 2) * i, 114.0f + m_nSelectIndex[i] * 71.0f, 0.0f));
	}
}

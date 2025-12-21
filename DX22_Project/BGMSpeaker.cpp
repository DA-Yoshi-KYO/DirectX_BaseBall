#include "BGMSpeaker.h"

CBGMSpeaker::CBGMSpeaker()
	: CGameObject()
	, m_pAudio(nullptr), m_cPath("")
{

}

CBGMSpeaker::~CBGMSpeaker()
{

}

void CBGMSpeaker::Init()
{
	m_pAudio = AddComponent<CAudio>();
}

void CBGMSpeaker::Init(SceneKind kind)
{
	switch (kind)
	{
	case SceneKind::Title:
		strcpy(m_cPath,PATH_BGM("Title.wav"));
		break;
	case SceneKind::TeamSelect:
		strcpy(m_cPath,PATH_BGM("TeamSelect.wav"));
		break;
	case SceneKind::MemberSelect:
		strcpy(m_cPath, PATH_BGM("MemberSelect.wav"));
		break;
	case SceneKind::Game:
		strcpy(m_cPath ,PATH_BGM("Game.wav"));
		break;
	default:
		break;
	}
	m_pAudio->Load(m_cPath);
	m_pAudio->SetVolume(0.2f);
	m_pAudio->Play(true);
}

int CBGMSpeaker::Inspecter(bool isEnd)
{
	// 子要素の数
	int nChildCnt = 0;

	// IMGUIウィンドウの初期化
	ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH - 300, 20));
	ImGui::SetNextWindowSize(ImVec2(280, 260));
	ImGui::Begin("Inspecter");

	// 子要素の初期化
	ImGui::BeginChild(ImGui::GetID((void*)nChildCnt), ImVec2(250, 30), ImGuiWindowFlags_NoTitleBar);

	// インスペクターに名前を表示
	ObjectID id = m_tID;
	std::string name = id.m_sName;

	// 同オブジェクトが2つ以上ある場合、そのindexも名前に表示する
	if (id.m_nSameCount != 0) name += std::to_string(id.m_nSameCount);
	name = "Name:" + name;
	ImGui::Text(name.c_str());

	// 子要素の終了
	ImGui::EndChild();
	// 子要素の数をインクリメント
	nChildCnt++;

	// 子要素の初期化
	ImGui::BeginChild(ImGui::GetID((void*)nChildCnt), ImVec2(250, 190), ImGuiWindowFlags_NoTitleBar);

	ImGui::InputText("BGMPath", m_cPath, 256);
	
	if (ImGui::Button("LoadAndPlay"))
	{
		m_pAudio->Uninit();
		m_pAudio->Load(m_cPath);
		m_pAudio->Play();
	}

	// 子要素の終了
	ImGui::EndChild();
	// 子要素の数をインクリメント
	nChildCnt++;

	// IMGUIウィンドウの終了
	if (isEnd) ImGui::End();

	// 子要素の数を返し、派生先のインスペクターに使用する
	return nChildCnt;
}

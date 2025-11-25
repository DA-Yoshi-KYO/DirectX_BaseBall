#include "Score.h"
#include "SpriteRenderer.h"
#include "DebugSystem.h"
#include "imgui.h"

#undef min

constexpr int ce_nMaxScore = 99;
constexpr DirectX::XMFLOAT3 ce_fScoreSize = { 50.0f,50.0f,0.0f };
constexpr DirectX::XMFLOAT2 ce_fScoreAjust = { 30.0f,60.0f };

CScore::CScore()
	: m_nScore(0), m_f2ScoreUVPos{}, m_f3ScoreOnePos{}
{
	m_tParam.m_f2UVSize = DirectX::XMFLOAT2(1.0f / 5.0f, 1.0f / 5.0f);
}

CScore::~CScore()
{

}

void CScore::Init()
{

	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("BallCountSheet.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite.cso"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite.cso"));

	m_tParam.m_f3Size = ce_fScoreSize;
}

void CScore::Update()
{
	int nNum = 0;

	// 1の位
	nNum = m_nScore % 10;
	m_f2ScoreUVPos[0] = DirectX::XMFLOAT2((nNum % 5) * m_tParam.m_f2UVSize.x, (nNum / 5) * m_tParam.m_f2UVSize.y);
	// 10の位
	nNum = m_nScore / 10;
	m_f2ScoreUVPos[1] = DirectX::XMFLOAT2((nNum % 5) * m_tParam.m_f2UVSize.x, (nNum / 5) * m_tParam.m_f2UVSize.y);
}

void CScore::Draw()
{
	for (int i = 0; i < 2; i++)
	{
		if (i == 1 && m_nScore < 10) continue;
		m_tParam.m_f3Pos = { m_f3ScoreOnePos.x - ce_fScoreAjust.x * i, m_f3ScoreOnePos.y,0.0f };
		m_tParam.m_f2UVPos = m_f2ScoreUVPos[i];
		CGameObject::Draw();
	}
}

void CScore::AddScore()
{
	m_nScore++;
	m_nScore = std::min(m_nScore, ce_nMaxScore);
}

int CScore::Inspecter(bool isEnd)
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

    if (CDebugSystem::GetInstance()->IsUpdate())
    {
        if (ImGui::CollapsingHeader(std::string("ScoreTransform").c_str()))
        {
            // アップデートが有効な時はパラメータの表示のみ行う
            // トランスフォームの表示
                // 座標の表示
            ImGui::Text(std::string("Position").c_str());
            DirectX::XMFLOAT3 pos = m_f3ScoreOnePos;
            ImGui::Text("PosX: %.3f", pos.x);
            ImGui::Text("PosY: %.3f", pos.y);
            ImGui::Text("PosZ: %.3f", pos.z);
            ImGui::Spacing();


        }
    }
    else
    {
        // アップデートが無効な時はパラメータの変更も行う
        // トランスフォームの表示
        if (ImGui::CollapsingHeader(std::string("ScoreTransform").c_str()))
        {
            // 座標の表示と変更
            DirectX::XMFLOAT3* pos = &m_f3ScoreOnePos;
            float inputPos[3] = { pos->x,pos->y,pos->z };
            ImGui::InputFloat3("InningPosition", inputPos);
            ImGui::Spacing();
            *pos = DirectX::XMFLOAT3(inputPos[0], inputPos[1], inputPos[2]);
        }

    }

    if (ImGui::CollapsingHeader(std::string("Status").c_str()))
    {
        ImGui::InputInt("InningNum", &m_nScore);
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

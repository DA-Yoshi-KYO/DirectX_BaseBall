#include "Inning.h"
#include "SpriteRenderer.h"
#include "DebugSystem.h"

constexpr DirectX::XMFLOAT3 ce_fInningSize = { 50.0f,50.0f,0.0f };
// イニングのパラメータ
constexpr DirectX::XMFLOAT3 ce_fInningPos = { 1020.0f,542.0f,0.0f };
constexpr DirectX::XMFLOAT3 ce_fInningAjust = { 30.0f,60.0f,0.0f };
// オモテ・ウラのパラメータ
constexpr DirectX::XMFLOAT3 ce_fTopBottomPos = { 1060.000,550.0f,0.0f };
constexpr DirectX::XMFLOAT3 ce_fTopBottomSize = { 50.0f,50.0f,0.0f };
constexpr DirectX::XMFLOAT3 ce_fTopBottomAjust = { 30.0f,60.0f,0.0f };
constexpr int ce_nSheetSplit = 5;

CInning::CInning()
	: m_nInningNum(1), m_bIsStart(true)
{
	m_tParam.m_f2UVSize = DirectX::XMFLOAT2(1.0f / 5.0f, 1.0f / 5.0f);
    m_f3InningPos = ce_fInningPos;
    m_f3TopBottomPos = ce_fTopBottomPos;
}

CInning::~CInning()
{

}

void CInning::Init()
{
	CSpriteRenderer* pRenderer = AddComponent<CSpriteRenderer>();
	pRenderer->Load(PATH_TEX("BallCountSheet.png"));
	pRenderer->LoadVertexShader(PATH_SHADER("VS_Sprite.cso"));
	pRenderer->LoadPixelShader(PATH_SHADER("PS_Sprite.cso"));

	m_tParam.m_f3Size = ce_fInningSize;
}

void CInning::Draw()
{
	// イニング数(1の位)の描画
	int nNum = m_nInningNum % 10;
	m_tParam.m_f3Pos = m_f3InningPos;
	m_tParam.m_f3Size = ce_fTopBottomSize;
	m_tParam.m_f2UVPos = { (float)(nNum % ce_nSheetSplit) / (float)ce_nSheetSplit ,(float)(nNum / ce_nSheetSplit) / (float)ce_nSheetSplit };
	CGameObject::Draw();

	// イニング数(10の位)の描画
	if (m_nInningNum >= 10)
	{
		nNum = m_nInningNum / 10;
		m_tParam.m_f3Pos = DirectX::XMFLOAT3(m_f3InningPos.x - ce_fInningAjust.x, m_f3InningPos.y, ce_fInningPos.z);
		m_tParam.m_f2UVPos = { (float)(nNum % ce_nSheetSplit) / (float)ce_nSheetSplit ,(float)(nNum / ce_nSheetSplit) / (float)ce_nSheetSplit };
		CGameObject::Draw();
	}

	// 表裏の描画
	m_tParam.m_f3Pos = m_f3TopBottomPos;
	m_tParam.m_f3Size = ce_fTopBottomSize;
	if (m_bIsStart) m_tParam.m_f2UVPos = { 2.0f / (float)ce_nSheetSplit ,2.0f / (float)ce_nSheetSplit };
	else m_tParam.m_f2UVPos = { 3.0f / (float)ce_nSheetSplit ,2.0f / (float)ce_nSheetSplit };
	CGameObject::Draw();
}

void CInning::InningProgress()
{
	if (!m_bIsStart)
	{
		m_nInningNum++;
	}

	m_bIsStart ^= true;
}

int CInning::Inspecter(bool isEnd)
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
        if (ImGui::CollapsingHeader(std::string("Inning").c_str()))
        {
            // アップデートが有効な時はパラメータの表示のみ行う
            // トランスフォームの表示
                // 座標の表示
            ImGui::Text(std::string("Position").c_str());
            DirectX::XMFLOAT3 pos = m_f3InningPos;
            ImGui::Text("PosX: %.3f", pos.x);
            ImGui::Text("PosY: %.3f", pos.y);
            ImGui::Text("PosZ: %.3f", pos.z);
            ImGui::Spacing();


        }
        if (ImGui::CollapsingHeader(std::string("TopBottom").c_str()))
        {
            // アップデートが有効な時はパラメータの表示のみ行う
            // トランスフォームの表示
                // 座標の表示
            ImGui::Text(std::string("Position").c_str());
            DirectX::XMFLOAT3 pos = m_f3TopBottomPos;
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
        if (ImGui::CollapsingHeader(std::string("Inning").c_str()))
        {
            // 座標の表示と変更
            DirectX::XMFLOAT3* pos = &m_f3InningPos;
            float inputPos[3] = { pos->x,pos->y,pos->z };
            ImGui::InputFloat3("InningPosition", inputPos);
            ImGui::Spacing();
            *pos = DirectX::XMFLOAT3(inputPos[0], inputPos[1], inputPos[2]);
        }
        if (ImGui::CollapsingHeader(std::string("TopBottom").c_str()))
        {
            // 座標の表示と変更
            DirectX::XMFLOAT3* pos = &m_f3TopBottomPos;
            float inputPos[3] = { pos->x,pos->y,pos->z };
            ImGui::InputFloat3("TopBottomPosition", inputPos);
            ImGui::Spacing();
            *pos = DirectX::XMFLOAT3(inputPos[0], inputPos[1], inputPos[2]);
        }
    }

    if (ImGui::CollapsingHeader(std::string("Status").c_str()))
    {
        ImGui::InputInt("InningNum", &m_nInningNum);
        ImGui::Checkbox("Top", &m_bIsStart);
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

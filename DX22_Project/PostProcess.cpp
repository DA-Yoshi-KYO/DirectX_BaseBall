#include "PostProcess.h"
#include "imgui.h"
#include "Defines.h"

CPostProcess::CPostProcess()
    : m_bIsActive(false), m_pPS(nullptr), m_nOrderNo(-1)
{

}

CPostProcess::~CPostProcess()
{
    SAFE_DELETE(m_pPS);
}

void CPostProcess::Write()
{

}

void CPostProcess::Load(std::string inPath)
{
    if (m_pPS) return;

    m_pPS = new PixelShader();
    if (FAILED(m_pPS->Load(inPath.c_str())))
    { 
        MessageBox(NULL, inPath.c_str(), "LoadError", MB_OK);
    }
}

void CPostProcess::ParameterSetting(bool isEnd)
{
    // 子要素の数
    int nChildCnt = 0;

    // IMGUIウィンドウの初期化
    ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH - 300, 280));
    ImGui::SetNextWindowSize(ImVec2(280, 250));
    ImGui::Begin("PostProcessParameter");

    // 子要素の初期化
    ImGui::BeginChild(ImGui::GetID((void*)nChildCnt), ImVec2(250, 30), ImGuiWindowFlags_NoTitleBar);
    std::string name = "Name:" + m_sName;
    ImGui::Text(name.c_str());

    // 子要素の終了
    ImGui::EndChild();
    // 子要素の数をインクリメント
    nChildCnt++;

    // 子要素の初期化
    ImGui::BeginChild(ImGui::GetID((void*)nChildCnt), ImVec2(250, 35), ImGuiWindowFlags_NoTitleBar);

    // アクティブ状態の切り替え
    ImGui::Checkbox(std::string("Active").c_str(), &m_bIsActive);

    ImGui::EndChild();

    // 子要素の数をインクリメント
    nChildCnt++;

    if (isEnd)
    {
        // IMGUIウィンドウの終了
        ImGui::End();
    }
    else
    {
        ImGui::BeginChild(ImGui::GetID((void*)nChildCnt), ImVec2(250, 140), ImGuiWindowFlags_NoTitleBar);
    }
}

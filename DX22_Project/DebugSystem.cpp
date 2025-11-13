#include "DebugSystem.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "DirectX.h"
#include "Main.h"
#include "Oparation.h"
#include "CameraManager.h"
#include "Input.h"
#include "PostProcessChain.h"
#include "PostProcess.h"
#include "GameObject.h"
#include <algorithm>

CDebugSystem* CDebugSystem::m_pInstance = nullptr;

CDebugSystem::CDebugSystem()
    : m_pObject(nullptr), m_bUpdate(true), m_pPostProcess(nullptr)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplWin32_Init(GetMyWindow());
    ImGui_ImplDX11_Init(GetDevice(), GetContext());
}

CDebugSystem::~CDebugSystem()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void CDebugSystem::Update()
{

}

void CDebugSystem::Draw()
{
    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    DrawHierarchy();
    DrawCameraParam();
    DrawUpdateTick();
    DrawSceneSelect();
    DrawCollision();
    DrawMousePos();
    DrawFPS();
    DrawPostProcess();
    DrawActivePostProcess();
    if (m_pObject) m_pObject->Inspecter();
    if (m_pPostProcess) m_pPostProcess->ParameterSetting();

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void CDebugSystem::DrawHierarchy()
{
    ImGui::SetNextWindowPos(ImVec2(20, 20));
    ImGui::SetNextWindowSize(ImVec2(280, 200));
    ImGui::Begin("Hierarchy");
    ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 160), ImGuiWindowFlags_NoTitleBar);

    auto Objects = GetScene()->GetIDVec();

    std::list<ObjectID> objectIDList{};
    for (auto Id : Objects)
    {
        objectIDList.push_back(Id);
    }

    objectIDList.sort([](ObjectID a, ObjectID b)
        {
            return a.m_nSameCount < b.m_nSameCount;
        });

    objectIDList.sort([](ObjectID a, ObjectID b)
        {
            return a.m_sName < b.m_sName;
        });

    for (auto itr = objectIDList.begin(); itr != objectIDList.end();)
    {
        std::string name = itr->m_sName;

        int nItrCount = 0;
        for (auto idItr : objectIDList)
        {
            if (idItr.m_sName == name)
            {
                nItrCount++;
            }
        }
        ObjectID id;
        id.m_sName = name;

            if (ImGui::CollapsingHeader(std::string("[" + name + "]").c_str()))
            {
                for (int i = 0; i < nItrCount; i++)
                {
                    std::string sButtonName = name;
                    if (i != 0) sButtonName += std::to_string(i);
                    id.m_nSameCount = i;
                    if (ImGui::Button(sButtonName.c_str()))
                    {
                        m_pObject = GetScene()->GetGameObject(id);
                    }

                }

            }
        

        std::advance(itr, nItrCount);
    }

    ImGui::EndChild();
    ImGui::End();
}

void CDebugSystem::DrawCameraParam()
{
    CCameraBase* pCamera = CCameraManager::GetInstance()->GetMainCamera();
    ImGui::SetNextWindowPos(ImVec2(20, SCREEN_HEIGHT - 500));
    ImGui::SetNextWindowSize(ImVec2(280, 200));
    ImGui::Begin("Camera");
    ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 160), ImGuiWindowFlags_NoTitleBar);

    if (ImGui::CollapsingHeader(std::string("[Transform]").c_str()))
    {
        ImGui::Text(std::string("Position").c_str());
        DirectX::XMFLOAT3 pos = pCamera->GetPos();
        ImGui::Text("PosX: %.3f", pos.x);
        ImGui::Text("PosY: %.3f", pos.y);
        ImGui::Text("PosZ: %.3f", pos.z);
        ImGui::Spacing();

        ImGui::Text(std::string("Look").c_str());
        DirectX::XMFLOAT3 look = pCamera->GetLook();
        ImGui::Text("LookX: %.3f", look.x);
        ImGui::Text("LookY: %.3f", look.y);
        ImGui::Text("LookZ: %.3f", look.z);
        ImGui::Spacing();

        ImGui::Text(std::string("UpVector").c_str());
        DirectX::XMFLOAT3 up = pCamera->GetUp();
        ImGui::Text("UpX: %.3f", up.x);
        ImGui::Text("UpY: %.3f", up.y);
        ImGui::Text("UpZ: %.3f", up.z);
        ImGui::Spacing();
    }

    ImGui::EndChild();
    ImGui::End();
}

void CDebugSystem::DrawUpdateTick()
{
    ImGui::SetNextWindowPos(ImVec2(20,SCREEN_HEIGHT - 100));
    ImGui::SetNextWindowSize(ImVec2(280, 80));
    ImGui::Begin("UpdateTick");

    ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(ce_f2InspecterSize.x,35.0f), ImGuiWindowFlags_NoTitleBar);
    ImGui::Checkbox("Use Update", &m_bUpdate);
    ImGui::EndChild();

    if (!m_bUpdate)
    {
        ImGui::BeginChild(ImGui::GetID((void*)1), ImVec2(ce_f2InspecterSize), ImGuiWindowFlags_NoTitleBar);
        if (ImGui::Button("Step"))
        {
            GetScene()->Update();
        }
        ImGui::EndChild();
    }

    ImGui::End();
}

void CDebugSystem::DrawSceneSelect()
{
    /*
    ImGui::SetNextWindowPos(ImVec2(20, SCREEN_HEIGHT - 220));
    ImGui::SetNextWindowSize(ImVec2(280, 100));
    ImGui::Begin("Scene");

    ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(ce_f2InspecterSize), ImGuiWindowFlags_NoTitleBar);
    static int nSelect = 0;
    ImGui::Combo("kind", &nSelect, "Title\0JobSelect\0Game\0Result\0");
    ImGui::EndChild();

    if (ImGui::Button("Go"))
    {
        switch (nSelect)
        {
        case 0:
            ChangeScene(new CSceneTitle());
            break;
        case 1:
            ChangeScene(new CSceneJobSelect());
            break;
        case 2:
            ChangeScene(new CSceneGame());
            break;
        case 3:
            ChangeScene(new CSceneResult());
            break;
        default:
            break;
        }
    }

    ImGui::End();
    */
}

void CDebugSystem::DrawCollision()
{
    /*
    ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH - 300, SCREEN_HEIGHT - 120));
    ImGui::SetNextWindowSize(ImVec2(280, 100));
    ImGui::Begin("Collision");

    ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(ce_f2InspecterSize), ImGuiWindowFlags_NoTitleBar);
    ImGui::Checkbox("DrawCollision", &m_bCollision);
    ImGui::EndChild();
    ImGui::End();
    if (!m_bCollision)return;

    auto CollisionVec = GetScene()->GetCollisionVec();
    for (int i = 0; i < CollisionVec.size(); i++)
    {
        CollisionVec[i]->Draw();
    }
    */
}

void CDebugSystem::DrawMousePos()
{
    ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH / 2 - 140, 20.0f));
    ImGui::SetNextWindowSize(ImVec2(280, 70));
    ImGui::Begin("Mouse");
    ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(ce_f2InspecterSize), ImGuiWindowFlags_NoTitleBar);

    POINT mousePos = *GetMousePosition();
    ImGui::Text(std::string("MouseX:" + std::to_string(mousePos.x)).c_str());
    ImGui::SameLine();
    ImGui::Text(std::string("MouseY:" + std::to_string(mousePos.y)).c_str());

    ImGui::EndChild();
    ImGui::End();
}

void CDebugSystem::DrawFPS()
{
    ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH / 2 + 170, 20.0f));
    ImGui::SetNextWindowSize(ImVec2(140, 70));
    ImGui::Begin("FPS");
    ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(120.0f,30.0f), ImGuiWindowFlags_NoTitleBar);

    int fps = GetFPS();
    ImGui::Text(std::string("FPS:" + std::to_string(fps)).c_str());
    
    ImGui::EndChild();
    ImGui::End();
}

void CDebugSystem::DrawPostProcess()
{
    ImGui::SetNextWindowPos(ImVec2(20, SCREEN_HEIGHT - 300));
    ImGui::SetNextWindowSize(ImVec2(280, 200));
    ImGui::Begin("PostProcess");
    ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 160), ImGuiWindowFlags_NoTitleBar);

    CPostProcessChain* pPostProcessChain = CPostProcessChain::GetInstance();
    std::vector<CPostProcess*> pPostProcessList = pPostProcessChain->GetAllPostProcessObject();

    for (int i = 0; i < pPostProcessList.size(); i++)
    {
        std::string sButtonName = pPostProcessList[i]->GetName();

        if (ImGui::Button(sButtonName.c_str()))
        {
            m_pPostProcess = pPostProcessList[i];
        }
    }

    ImGui::EndChild();
    ImGui::End();
}

void CDebugSystem::DrawActivePostProcess()
{
    //ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH - 300, 530));
    //ImGui::SetNextWindowSize(ImVec2(280, 170));
    //ImGui::Begin("ActivePostProcess");
    //ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 135), ImGuiWindowFlags_NoTitleBar);

    //CPostProcessChain* pPostProcessChain = CPostProcessChain::GetInstance();
    //std::vector<CPostProcess*> pActivePostProcess = pPostProcessChain->GetAllPostProcessObject();
    //std::vector<std::string> effects;
    //effects.clear();
    //for (int i = 0; i < (pActivePostProcess).size(); i++)
    //{
    //    effects.push_back((pActivePostProcess)[i]->GetName());
    //}

    //static int current = 0;
    //current = std::min(current, int(effects.size() - 1));

    //if (!effects.empty())
    //{
    //    ImGui::ListBox("Effects", &current,
    //        [](void* data, int idx, const char** out_text)
    //        {
    //            auto& list = *(std::vector<std::string>*)data;
    //            *out_text = list[idx].c_str();
    //            return true;
    //        },
    //        (void*)&effects, effects.size());
    //}

    //ImGui::EndChild();
    //ImGui::End();
}

CDebugSystem* CDebugSystem::GetInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new CDebugSystem();
    }
    return m_pInstance;
}

void CDebugSystem::Release()
{
    if (m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}

void CDebugSystem::ReleaseGameObject()
{
    m_pObject = nullptr;
}

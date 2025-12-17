#include "DebugSystem.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "DirectX.h"
#include "Main.h"
#include "Oparation.h"
#include "CameraDebug.h"
#include "Input.h"
#include "PostProcessChain.h"
#include "PostProcess.h"
#include "GameObject.h"
#include <algorithm>
#include <unordered_map>
#include "CollisionBox.h"
#include "CollisionTriangle.h"

CDebugSystem* CDebugSystem::m_pInstance = nullptr;

CDebugSystem::CDebugSystem()
    : m_pObject(nullptr), m_bUpdate(true),m_bCollision(false), m_pPostProcess(nullptr), m_bCameraMove(true)
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
    DrawCollisionInfo();
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
    CCamera* pCamera = CCamera::GetInstance();
    ImGui::Begin("Camera");
    ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 160), ImGuiWindowFlags_NoTitleBar);
    
    if (m_bUpdate)
    {
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
    }
    else
    {
        if (ImGui::CollapsingHeader(std::string("[Transform]").c_str()))
        {
            DirectX::XMFLOAT3 pos = pCamera->GetPos();
            float inputPos[3] = { pos.x,pos.y,pos.z };
            ImGui::InputFloat3("Position", inputPos);
            ImGui::Spacing();
            pos = DirectX::XMFLOAT3(inputPos[0], inputPos[1], inputPos[2]);
            pCamera->SetPos(pos);

            ImGui::Text(std::string("Look").c_str());
            DirectX::XMFLOAT3 look = pCamera->GetLook();
            float inputLook[3] = { look.x,look.y,look.z };
            ImGui::InputFloat3("Look", inputLook);
            ImGui::Spacing();
            look = DirectX::XMFLOAT3(inputLook[0], inputLook[1], inputLook[2]);
            pCamera->SetLook(look);

            ImGui::Text(std::string("UpVector").c_str());
            DirectX::XMFLOAT3 up = pCamera->GetUp();
            float inputUp[3] = { up.x,up.y,up.z };
            ImGui::InputFloat3("Up", inputUp);
            ImGui::Spacing();
            look = DirectX::XMFLOAT3(inputUp[0], inputUp[1], inputUp[2]);
            pCamera->SetUp(up);

            ImGui::Checkbox("CameraMove", &m_bCameraMove);
            dynamic_cast<CCameraDebug*>(pCamera)->SetMove(m_bCameraMove);
        }
    }

    ImGui::EndChild();
    ImGui::End();
}

void CDebugSystem::DrawUpdateTick()
{
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
    ImGui::Begin("Collision");

    ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(ce_f2InspecterSize), ImGuiWindowFlags_NoTitleBar);
    ImGui::Checkbox("DrawCollision", &m_bCollision);
    ImGui::EndChild();
    ImGui::End();
    if (!m_bCollision)return;

    auto CollisionVec = GetScene()->GetCollisionList();
    for (int i = 0; i < CollisionVec.size(); i++)
    {
        CollisionVec[i]->Draw();
    }
    
}
void CDebugSystem::DrawMousePos()
{
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
    ImGui::Begin("FPS");
    ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(120.0f,30.0f), ImGuiWindowFlags_NoTitleBar);

    int fps = GetFPS();
    ImGui::Text(std::string("FPS:" + std::to_string(fps)).c_str());
    
    ImGui::EndChild();
    ImGui::End();
}

void CDebugSystem::DrawPostProcess()
{
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

void CDebugSystem::DrawCollisionInfo()
{
    ImGui::Begin("CollisionInfo");
    ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 160), ImGuiWindowFlags_NoTitleBar);

    CScene* pScene = GetScene();
    auto CollisionList = pScene->GetCollisionList();
    std::unordered_map<std::string, int> sameNameNum;
    std::unordered_map<CCollisionBase*, std::vector<std::string>> sameCollisionMap;

    for (auto itr : CollisionList)
    {
        std::string name = itr->GetTag();
        if (sameNameNum.find(name) == sameNameNum.end()) sameNameNum.emplace(name,0);
        else sameNameNum[name]++;

        std::string buttonName = "[" + name + "]";
        sameCollisionMap[itr].push_back(name + std::to_string(sameNameNum[name]));
    }

    for (auto itr : sameCollisionMap)
    {
        for (auto itrButton : itr.second)
        {
            if (ImGui::CollapsingHeader(itrButton.c_str()))
            {
                Collision::Info info = itr.first->GetInfo();
                float inputF3[3]{};
                switch (info.type)
                {
                case Collision::Type::eBox:
                    inputF3[0] = info.box.center.x;
                    inputF3[1] = info.box.center.y;
                    inputF3[2] = info.box.center.z;
                    ImGui::InputFloat3("Center", inputF3);
                    ImGui::Spacing();
                    info.box.center = DirectX::XMFLOAT3(inputF3[0], inputF3[1], inputF3[2]);

                    inputF3[0] = info.box.size.x;
                    inputF3[1] = info.box.size.y;
                    inputF3[2] = info.box.size.z;
                    ImGui::InputFloat3("Size", inputF3);
                    ImGui::Spacing();
                    info.box.size = DirectX::XMFLOAT3(inputF3[0], inputF3[1], inputF3[2]);

                    dynamic_cast<CCollisionBox*>(itr.first)->SetInfo(info.box.center, info.box.size);
                    break;
                case Collision::Type::eTriangle:
                    inputF3[0] = info.triangle.point[0].x;
                    inputF3[1] = info.triangle.point[0].y;
                    inputF3[2] = info.triangle.point[0].z;
                    ImGui::InputFloat3("Point1", inputF3);
                    ImGui::Spacing();
                    info.triangle.point[0] = DirectX::XMFLOAT3(inputF3[0], inputF3[1], inputF3[2]);

                    inputF3[0] = info.triangle.point[1].x;
                    inputF3[1] = info.triangle.point[1].y;
                    inputF3[2] = info.triangle.point[1].z;
                    ImGui::InputFloat3("Point2", inputF3);
                    ImGui::Spacing();
                    info.triangle.point[1] = DirectX::XMFLOAT3(inputF3[0], inputF3[1], inputF3[2]);

                    inputF3[0] = info.triangle.point[2].x;
                    inputF3[1] = info.triangle.point[2].y;
                    inputF3[2] = info.triangle.point[2].z;
                    ImGui::InputFloat3("Point3", inputF3);
                    ImGui::Spacing();
                    info.triangle.point[2] = DirectX::XMFLOAT3(inputF3[0], inputF3[1], inputF3[2]);

                    dynamic_cast<CCollisionTriangle*>(itr.first)->SetInfo(info.triangle.point[0], info.triangle.point[1], info.triangle.point[2]);
                    break;

                default:
                    break;
                }
            }
        }
    }

    ImGui::EndChild();
    ImGui::End();

    ImGui::Begin("CollisionInfo");
    ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 160), ImGuiWindowFlags_NoTitleBar);



    ImGui::EndChild();
    ImGui::End();
}

void CDebugSystem::DrawActivePostProcess()
{
    ImGui::Begin("ActivePostProcess");
    ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 135), ImGuiWindowFlags_NoTitleBar);

    CPostProcessChain* pPostProcessChain = CPostProcessChain::GetInstance();
    std::vector<CPostProcess*> pActivePostProcess = pPostProcessChain->GetAllPostProcessObject();
    std::vector<std::string> effects;
    effects.clear();
    for (int i = 0; i < (pActivePostProcess).size(); i++)
    {
        effects.push_back((pActivePostProcess)[i]->GetName());
    }

    static int current = 0;
    current = std::min(current, int(effects.size() - 1));

    if (!effects.empty())
    {
        ImGui::ListBox("Effects", &current,
            [](void* data, int idx, const char** out_text)
            {
                auto& list = *(std::vector<std::string>*)data;
                *out_text = list[idx].c_str();
                return true;
            },
            (void*)&effects, effects.size());
    }

    ImGui::EndChild();
    ImGui::End();
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

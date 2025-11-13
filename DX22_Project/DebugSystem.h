#pragma once


#include "imgui.h"

constexpr ImVec2 ce_f2InspecterSize = ImVec2(250, 30);

class CDebugSystem
{
private:
    CDebugSystem();
public:
    ~CDebugSystem();
    CDebugSystem(const CDebugSystem&) = delete;
    CDebugSystem& operator=(const CDebugSystem&) = delete;
    static CDebugSystem* GetInstance();
    void Release();
    void ReleaseGameObject();

    void Update();
    void Draw();

    bool IsUpdate() { return m_bUpdate; }
private:
    static CDebugSystem* m_pInstance;
    class CGameObject* m_pObject;
    class CPostProcess* m_pPostProcess;
    bool m_bUpdate;

private:
    void DrawHierarchy();
    void DrawCameraParam();
    void DrawUpdateTick();
    void DrawSceneSelect();
    void DrawCollision();
    void DrawMousePos();
    void DrawFPS();
    void DrawPostProcess();
    void DrawActivePostProcess();

};

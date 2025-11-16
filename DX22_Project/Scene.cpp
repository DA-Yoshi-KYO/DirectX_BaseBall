#include "GameObject.h"
#include "Scene.h"
#include "Camera.h"
#include "Geometory.h"
#include "PostProcessChain.h"
#include "DebugSystem.h"

CScene::CScene()
{

}

CScene::~CScene()
{

}

void CScene::Init()
{

}

void CScene::Uninit()
{
	for (auto list : m_pGameObject_List)
	{
        for (auto obj : list)
        {
            obj->Uninit();
            delete obj;
        }
        list.clear();
	}
    CPostProcessChain::GetInstance()->ClearPostProcessList();
    CDebugSystem::GetInstance()->ReleaseGameObject();

    m_tIDVec.clear();
}

void CScene::Update()
{
    for (auto& list : m_pGameObject_List)
    {
        for (auto obj : list)
        {
            obj->Update();
        }
    }

    for (auto itr = m_tIDVec.begin(); itr != m_tIDVec.end();)
    {
        if (GetGameObject(*itr)->IsDestroy())
        {
            itr = m_tIDVec.erase(itr);
        }
        else
        {
            itr++;
        }
    }

    for (auto& list : m_pGameObject_List)
    {
        list.remove_if([](CGameObject* pObj)
            {
                if (pObj->IsDestroy())
                {
                    pObj->OnDestroy();
                    pObj->Uninit();
                    delete pObj;
                    pObj = nullptr;

                    return true;
                }
                return false;
            });

    }
}

void CScene::Draw()
{
    // 描画前にレンダーターゲットをクリアする
    RenderTarget* pRTV = CPostProcessChain::GetInstance()->GetScreenTarget();
    DepthStencil* pDSV = GetDefaultDSV();
    float clear[4] = { 1.0f,1.0f, 1.0f, 1.0f };
    pRTV->Clear(clear);
    SetRenderTargets(1, &pRTV, pDSV);

    DrawGrid(); // グリッドの描画

    // シーンの描画
    for (auto& list : m_pGameObject_List)
    {
        for (auto& obj : list)
        {
            obj->Draw();
        }
    }

    // ポストプロセスの適用処理
    CPostProcessChain::GetInstance()->Attach();
}

std::vector<ObjectID> CScene::GetIDVec()
{
    return m_tIDVec;
}

std::array<std::list<CGameObject*>, (int)Tag::Max> CScene::GetGameObjectList()
{
    return m_pGameObject_List;
}

void CScene::DrawGrid()
{
    CCamera* pMainCamera = CCamera::GetInstance();
    Geometory::SetView(pMainCamera->GetViewMatrix());
    Geometory::SetProjection(pMainCamera->GetProjectionMatrix());

    // グリッド
    DirectX::XMFLOAT4 lineColor(0.5f, 0.5f, 0.5f, 1.0f);
    float size = DEBUG_GRID_NUM * DEBUG_GRID_MARGIN;
    for (int i = 1; i <= DEBUG_GRID_NUM; ++i)
    {
        float grid = i * DEBUG_GRID_MARGIN;
        DirectX::XMFLOAT3 pos[2] = {
            DirectX::XMFLOAT3(grid, 0.0f, size),
            DirectX::XMFLOAT3(grid, 0.0f,-size),
        };
        Geometory::AddLine(pos[0], pos[1], lineColor);
        pos[0].x = pos[1].x = -grid;
        Geometory::AddLine(pos[0], pos[1], lineColor);
        pos[0].x = size;
        pos[1].x = -size;
        pos[0].z = pos[1].z = grid;
        Geometory::AddLine(pos[0], pos[1], lineColor);
        pos[0].z = pos[1].z = -grid;
        Geometory::AddLine(pos[0], pos[1], lineColor);
    }
    // 軸
    Geometory::AddLine(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(size, 0, 0), DirectX::XMFLOAT4(1, 0, 0, 1));
    Geometory::AddLine(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, size, 0), DirectX::XMFLOAT4(0, 1, 0, 1));
    Geometory::AddLine(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 0, size), DirectX::XMFLOAT4(0, 0, 1, 1));
    Geometory::AddLine(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(-size, 0, 0), DirectX::XMFLOAT4(0, 0, 0, 1));
    Geometory::AddLine(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 0, -size), DirectX::XMFLOAT4(0, 0, 0, 1));

    Geometory::DrawLines();
}

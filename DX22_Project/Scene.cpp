#include "GameObject.h"
#include "Scene.h"
#include "Camera.h"
#include "Geometory.h"
#include "PostProcessChain.h"
#include "DebugSystem.h"

CScene::CScene()
    : m_bAnyObjectDestroy(false)
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
    CCamera::GetInstance()->Update();

    // オブジェクトの更新処理
    for (auto& list : m_pGameObject_List)
    {
        for (auto obj : list)
        {
            obj->Update();
        }
    }

    m_pHitCollisionList.clear();
    for (int i = 0; i < m_pCollisionList.size(); i++)
    {
        for (int j = i + 1; j < m_pCollisionList.size(); j++)
        {
            Collision::Result result = m_pCollisionList[i]->IsHit(m_pCollisionList[j]);
            if (result.isHit)
            {
                CGameObject* hitA = m_pCollisionList[i]->GetGameObject();
                CGameObject* hitB = m_pCollisionList[j]->GetGameObject();
                hitA->OnCollision(m_pCollisionList[j],m_pCollisionList[i]->GetTag(), result);
                hitB->OnCollision(m_pCollisionList[i],m_pCollisionList[j]->GetTag(), result);
                m_pHitCollisionList.push_back(std::pair(m_pCollisionList[i], m_pCollisionList[j]));
            }
        }
    }


    for (auto itr : m_pOldHitCollisionList)
    {
        auto findItr = std::find(m_pHitCollisionList.begin(), m_pHitCollisionList.end(), itr);

        if (findItr == m_pHitCollisionList.end())
        {
            itr.first->GetGameObject()->OnCollisionExit(itr.second, itr.first->GetTag());
            itr.second->GetGameObject()->OnCollisionExit(itr.first, itr.second->GetTag());
        }
    }
    m_pOldHitCollisionList = m_pHitCollisionList;

    // 何かしらのオブジェクトが削除されたと通知された際に、リストから削除する処理を行う
    if (m_bAnyObjectDestroy)
    {
        // 更新を受けて削除予定のオブジェクトがあればコリジョンを削除
        for (auto itr = m_pCollisionList.begin(); itr != m_pCollisionList.end();)
        {
            if ((*itr)->GetGameObject()->IsDestroy())
            {
                itr = m_pCollisionList.erase(itr);
            }
            else
            {
                itr++;
            }
        }

        // 更新を受けて削除予定のオブジェクトがあればIDを削除
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
        for (auto itr = m_pHitCollisionList.begin(); itr != m_pHitCollisionList.end();)
        {
            if ((*itr).first->GetGameObject()->IsDestroy() || (*itr).second->GetGameObject()->IsDestroy())
            {
                itr = m_pHitCollisionList.erase(itr);
            }
            else
            {
                itr++;
            }
        }
        for (auto itr = m_pOldHitCollisionList.begin(); itr != m_pOldHitCollisionList.end();)
        {
            if ((*itr).first->GetGameObject()->IsDestroy() || (*itr).second->GetGameObject()->IsDestroy())
            {
                itr = m_pOldHitCollisionList.erase(itr);
            }
            else
            {
                itr++;
            }
        }

        // 更新を受けて削除予定のオブジェクトがあれば削除
        for (auto& list : m_pGameObject_List)
        {
            list.remove_if([](CGameObject* pObj)
                {
                    if (pObj->IsDestroy())
                    {
                        CDebugSystem::GetInstance()->ReleaseGameObject();
                        pObj->OnDestroy();
                        pObj->Uninit();
                        delete pObj;
                        pObj = nullptr;

                        return true;
                    }
                    return false;
                });

        }

        m_bAnyObjectDestroy = false;
    }
}

void CScene::Draw()
{
    // 描画前にレンダーターゲットをクリアする
    // RenderTarget* pRTV = CPostProcessChain::GetInstance()->GetScreenTarget();
    RenderTarget* pRTV = GetDefaultRTV();
    DepthStencil* pDSV = GetDefaultDSV();
    float clear[4] = { 1.0f,1.0f, 1.0f, 1.0f };
    pRTV->Clear(clear);
    SetRenderTargets(1, &pRTV, pDSV);

    // DrawGrid(); // グリッドの描画

    // シーンの描画
    for (auto& list : m_pGameObject_List)
    {
        for (auto& obj : list)
        {
            obj->Draw();
        }
    }

    // ポストプロセスの適用処理
    //CPostProcessChain::GetInstance()->Attach();
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

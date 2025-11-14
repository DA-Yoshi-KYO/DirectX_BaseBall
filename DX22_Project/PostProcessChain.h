#pragma once

#include <vector>

class CPostProcess;
class RenderTarget;

class CPostProcessChain
{
private:
    CPostProcessChain();

public:
    CPostProcessChain(const CPostProcessChain&) = delete;
    CPostProcessChain& operator=(const CPostProcessChain&) = delete;
    void Attach();
    static CPostProcessChain* GetInstance();

    RenderTarget* GetScreenTarget();
    void Release();
    void ClearPostProcessList();

    template<typename T = CPostProcess>
    void AddPostProcess()
    {
        for (auto itr : m_pPostProcessList)
        {
            if (dynamic_cast<T*>(itr))
            {
                return;
            }
        }

        T* pAddPostProcess = new T();
        pAddPostProcess->Init();
        m_pPostProcessList.push_back(pAddPostProcess);
    }

    template<typename T = CPostProcess>
    T* GetPostProcessObject()
    {
        for (auto itr : m_pPostProcessList)
        {
            T* ret = dynamic_cast<T*>(itr);
            if (ret != nullptr) return ret;
        }
        return nullptr;
    }

    std::vector<CPostProcess*> GetAllPostProcessObject() { return m_pPostProcessList; }
    
private:
    std::vector<CPostProcess*> m_pPostProcessList;
    std::vector<CPostProcess*> m_pActivePostProcessList;
    RenderTarget* m_pScreenRTV;
    RenderTarget* m_pOutRTV;
    static CPostProcessChain* m_pInstance;

private:
    void SetMatrix();

};


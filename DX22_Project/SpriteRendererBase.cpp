#include "SpriteRendererBase.h"

std::map<std::string, Texture*> CSpriteRendererBase::m_pTextureList = {};

CSpriteRendererBase::~CSpriteRendererBase()
{

}

void CSpriteRendererBase::Load(std::string inPath)
{
    m_sTextureKey = inPath;

    auto itr = m_pTextureList.find(m_sTextureKey);
    if (itr != m_pTextureList.end())
    {
        return;
    }

    Texture* pTexture = new Texture();
    if (FAILED(pTexture->Create(m_sTextureKey.c_str())))
    {
        MessageBox(NULL, m_sTextureKey.c_str(), "Load failed.", MB_OK);
    }

    m_pTextureList[m_sTextureKey] = pTexture;
}

Texture* CSpriteRendererBase::GetTexture()
{
    return m_pTextureList[m_sTextureKey];
}

void CSpriteRendererBase::UnloadAll()
{
    for (auto itr : m_pTextureList)
    {
        if (itr.second)
        {
            delete itr.second;
            itr.second = nullptr;
        }
    }
}

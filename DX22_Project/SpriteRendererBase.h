#pragma once

#include "RendererComponent.h"
#include "Texture.h"

class CSpriteRendererBase : public CRendererComponent
{
public:
    using CRendererComponent::CRendererComponent;
    virtual ~CSpriteRendererBase();
    virtual void Draw() = 0;
    static void UnloadAll();
    void Load(std::string inPath);
    Texture* GetTexture();

protected:
    static std::map<std::string, Texture*> m_pTextureList;
    std::string m_sTextureKey;

};


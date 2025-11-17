#pragma once

#include "GameObject.h"
#include "SpriteRenderer.h"

class CTransition : public CGameObject
{
public:
    ~CTransition();
    void Init() override;
    void Update() override;

    void FadeIn(int inFlameCount, std::function<void()> onFadeInComplete);
    void FadeOut(int inFlameCount, std::function<void()> onFadeOutComplete);

public:
    enum eMode
    {
        NONE,
        FADE_IN,
        FADE_OUT
    };

private:
    CSpriteRenderer* m_pSprite;
    int m_nCurrentCount;
    int m_nEndCount;
    std::function<void()> m_fFadeComplete;
    eMode m_eCurrentMode;

};

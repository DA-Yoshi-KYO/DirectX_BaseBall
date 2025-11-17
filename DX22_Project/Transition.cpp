#include "Transition.h"

CTransition::~CTransition()
{
}

void CTransition::Init()
{
    m_pSprite = AddComponent<CSpriteRenderer>();
    m_pSprite->Load(PATH_TEX("Fade.png"));

    m_tParam.m_f3Size = DirectX::XMFLOAT3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
    m_tParam.m_f4Color = DirectX::XMFLOAT4(1.0f,1.0f,1.0f,0.0f);

    m_nCurrentCount = 0;
    m_nEndCount = 0;
    m_fFadeComplete = nullptr;
    m_eCurrentMode = eMode::NONE;
}

void CTransition::Update()
{
    if (m_nCurrentCount >= m_nEndCount)
    {
        m_eCurrentMode = eMode::NONE;

        if (m_fFadeComplete)
        {
            m_fFadeComplete();
            m_fFadeComplete = nullptr;
        }
        return;
    }

    m_nCurrentCount++;
    float alpha = 0.0f;
    switch (m_eCurrentMode)
    {
    case CTransition::NONE:
        break;
    case CTransition::FADE_IN:
        alpha = 1.0f - (float)m_nCurrentCount / m_nEndCount;
        break;
    case CTransition::FADE_OUT:
        alpha = (float)m_nCurrentCount / m_nEndCount;
        break;
    default:
        break;
    }

    m_tParam.m_f4Color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, alpha);
}

void CTransition::FadeIn(int inFlameCount, std::function<void()> onFadeInComplete)
{
    m_eCurrentMode = eMode::FADE_IN;
    m_nCurrentCount = 0;
    m_nEndCount = inFlameCount;
    m_fFadeComplete = onFadeInComplete;
}

void CTransition::FadeOut(int inFlameCount, std::function<void()> onFadeOutComplete)
{
    m_eCurrentMode = eMode::FADE_OUT;
    m_nCurrentCount = 0;
    m_nEndCount = inFlameCount;
    m_fFadeComplete = onFadeOutComplete;
}

#pragma once
#include "Texture.h" 
#include "Sprite.h" 
#include "GameUI.h"

class CGaugeUI : public CGameUI
{
public:
    // コンストラクタ 
    CGaugeUI();
    // デストラクタ 
    ~CGaugeUI() override;

    // 更新処理 
    void Update() override;

    // 描画処理 
    void Draw() override;

    // ゲージ量の設定 
    void SetGauge(float rate);
private:
    Texture* m_pFrameTex;
    Texture* m_pGaugeTex;
    float m_rate;
};
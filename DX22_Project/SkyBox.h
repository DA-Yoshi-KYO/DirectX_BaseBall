#pragma once

// インクルード部
#include "GameObject.h"

/// <summary>
/// スカイボックスの描画を行うクラス
/// </summary>
class CSkyBox : public CGameObject
{
public:
    CSkyBox();
    ~CSkyBox();
    void Init() override;
    void Update() override;
    
};

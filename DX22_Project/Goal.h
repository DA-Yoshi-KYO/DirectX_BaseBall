#pragma once

#include "Texture.h"
#include "GameObject.h"
#include "Camera.h"

class CGoal : public CGameObject 
{
public:  
    CGoal();
    ~CGoal() override;
    void Update() override;
    void Draw() override;

    bool IsHit(Collision::Info collision);

    void SetCamera(CCamera* camera);
    void SetPos(DirectX::XMFLOAT3 pos);

private:
    Texture* m_pGoalTex;
    CCamera* m_pCamera;
};
#pragma once

#include "PolylineEffect.h" 

class TrailEffect : public PolylineEffect 
{
public:
    // 初期化処理 
   // TrailEffect(CPlayer* pPlayer);
    // 終了処理 
    virtual ~TrailEffect();

protected:
    // ポリライン制御点更新処理 
    void UpdateControlPoints(LineID id, ControlPoints& controlPoints) final;

private:
    //CPlayer* m_pPlayer; // プレイヤー情報 
    DirectX::XMFLOAT3 m_oldPos; // 以前にプレイヤーがいた位置 
};
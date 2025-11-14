#pragma once
#include "Defines.h"
#include "Texture.h"
#include "StrikeZone.h"
#include "Collision.h"
#include "RendererComponent.h"
#include "GameObject.h"

constexpr DirectX::XMFLOAT3 ce_fPitchingCursorPos = { 0.0f,-150.0f,0.0f };	// 投球カーソルの初期位置
constexpr int ce_nMaxBenderQuality = 7;	// 細大変化量

class CPitching;

class CPitchingCursor : public CGameObject
{
public:
	CPitchingCursor();
	~CPitchingCursor();
	void Update();
	void Draw();

public:

	/// <summary> GetCollision:カーソルの当たり判定を取得する </summary>
	/// <param name="isCursorOnry"> true:当たり判定の座標だけ取得する false:当たり判定をそのまま取得する </param>
	/// <param name="type"> 当たり判定の形(def = 円形) </param>
	/// <returns> 投球カーソルの当たり判定 </returns>
	static Collision::Info2D GetCollision(bool isCursorOnry, Collision::Type2D type = Collision::Type2D::eCircle);
private:
	RendererParam m_tBallParam;
	RendererParam m_tPredParam;
	bool m_bMove;	//  カーソルを動かせるか
	static Collision::Info2D m_Collision;	// カーソルの当たり判定

};

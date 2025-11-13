// ==============================
//    インクルード部
// ==============================
#pragma once
#include "Texture.h"
#include "Defines.h"
#include "Collision.h"

// ==============================
//    定数定義
// ==============================
constexpr DirectX::XMFLOAT2 ce_fStrikeZoneSize{ 100.0f,100.0f };

// シングルトンクラス
class CStrikeZone
{
private:
	CStrikeZone();
public:
	~CStrikeZone();
	void Update();
	void Draw();

public:
	// ==============================
	//    アクセサ
	// ==============================
	// 
	// ------------Getter------------
	/// <summary> GetCollision:ストライクゾーンの当たり判定情報を取得する </summary>
	/// <returns> ストライクゾーンの当たり判定情報(Squere) </returns>
	Collision::Info2D GetCollision();
	/// <summary> GetPos:ストライクゾーンの座標を取得する </summary>
	/// <returns> ストライクゾーンの座標 </returns>
	DirectX::XMFLOAT2 GetPos();
	/// <summary> GetSize:ストライクゾーンのサイズを取得する </summary>
	/// <returns> ストライクゾーンのサイズ</returns>
	DirectX::XMFLOAT2 GetSize();
	
private:
	// コンポジション
	std::unique_ptr<Texture> m_pTexture;

	// パラメータ
	ObjectParam m_tParam;

	// 当たり判定情報
	Collision::Info2D m_Collision;

};
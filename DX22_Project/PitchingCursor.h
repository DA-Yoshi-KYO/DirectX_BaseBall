// ==============================
//    インクルード部
// ==============================
#pragma once
#include "Defines.h"
#include "Texture.h"
#include "StrikeZone.h"
#include "Collision.h"

// ==============================
//    定数定義
// ==============================
constexpr DirectX::XMFLOAT2 ce_fPitchingCursorPos = { 0.0f,-150.0f };	// 投球カーソルの初期位置

class CPitchingCursor
{
public:
	CPitchingCursor();
	~CPitchingCursor();
	void Update();
	void Draw();

public:
	// ==============================
	//    アクセサ
	// ==============================
	// 
	// ------------Setter------------
	/// <summary name="SetStrikeZone"> SetStrikeZone:CStrikeZoneクラスのインスタンスをセットする </summary>
	/// <param name="zone"> CStrikeZoneクラスのインスタンス </param>
	void SetStrikeZone(CStrikeZone* zone);
	/// <summary name="SetPos"> SetPos:カーソルの座標をセットする </summary>
	/// <param name="pos"> カーソルの座標 </param>
	void SetPos(DirectX::XMFLOAT2 pos);
	/// <summary name="SetMove"> SetMove:カーソルが動けるかをセットする </summary>
	/// <param name="isMove"> true:動ける false:動けない </param>
	void SetMove(bool isMove);

	// ------------Getter------------
	/// <summary> GetPos:投球カーソルの座標を取得する </summary>
	/// <returns> 投球カーソルの座標 </returns>
	DirectX::XMFLOAT2 GetPos();
	/// <summary> GetSize:投球カーソルのサイズを取得する </summary>
	/// <returns> 投球カーソルのサイズ </returns>
	DirectX::XMFLOAT2 GetSize();
	/// <summary> GetCollision:カーソルの当たり判定を取得する </summary>
	/// <param name="isCursorOnry"> true:当たり判定の座標だけ取得する false:当たり判定をそのまま取得する </param>
	/// <param name="type"> 当たり判定の形(def = 円形) </param>
	/// <returns> 投球カーソルの当たり判定 </returns>
	static Collision::Info2D GetCollision(bool isCursorOnry, Collision::Type2D type = Collision::Type2D::eCircle);
private:
	// コンポジション
	std::unique_ptr<CStrikeZone> m_pStrikeZone;
	std::unique_ptr<Texture> m_pTexture;
	// スプライトのパラメータ
	SpriteParam m_tParam;
	// メンバ変数
	bool m_bMove;	//  カーソルを動かせるか
	// 静的メンバ変数
	static Collision::Info2D m_Collision;	// カーソルの当たり判定
};

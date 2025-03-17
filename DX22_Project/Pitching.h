/*+===================================================================
	File: Pitching.h
	Summary: 投球処理
	Author: 吉田京志郎
	Date:	2025/03/14	初回作成
			2025/03/15	ピッチングのシステム追加
						ピッチングカーソルの移動・描画
			2025/03/17	ストライク・ボール判定を反映
===================================================================+*/

// ==============================
//    インクルード部
// ==============================
#pragma once
#include "Defines.h"
#include "StrikeZone.h"
#include "Cursor.h"
#include "Texture.h"

class CPitching
{
public:
	CPitching();
	~CPitching();
	void Update();
	void Draw();

	/// <summary> SetStrikeZone </summary>
	/// <param name="zone:"> CStrikeZoneクラスのインスタンス </param>
	void SetStrikeZone(CStrikeZone* zone);
	/// <summary> SetStrikeZone </summary>
	/// <param name="cursor:"> CCursorクラスのインスタンス </param>
	void SetCursor(CCursor* cursor);

	// 投球の流れ
	enum class PitchingPhase
	{
		Set,
		Pitch,
		Release,
	};
	int GetPitchingPhase();

	float GetChatchTime();
private:
	int m_nPitchingPhase;	// 投球の流れ

	// テクスチャ
	enum class TexKind
	{
		ReleasePoint,
		PitchingCircle,

		Max
	};
	std::unique_ptr<Texture> m_pTexture[(int)TexKind::Max];
	SpriteParam m_tParam[(int)TexKind::Max];
	
	// コンポジション
	std::unique_ptr<CStrikeZone> m_pStrikeZone;
	std::unique_ptr<CCursor> m_pCursor;

	// メンバ変数
	float m_fSpeed;			// 球速
	float m_fChatchTime;	// 捕球までの時間

	enum class PitchingQuality
	{
		Miss,
		Nomal,
		Nice,
		Best
	};
private:
	// 内部処理
	// リリースポイント、ピッチングサークルの描画
	void DrawCircle();
};
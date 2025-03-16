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
private:
	// コンポジション
	std::unique_ptr<CStrikeZone> m_pStrikeZone;
	std::unique_ptr<CCursor> m_pCursor;
	
	int m_nPitchingPhase;	// 投球の流れ
	float m_fSpeed;			// 球速
	float m_fChatchTime;	// 捕球までの時間

	// 投球の流れ
	enum class PitchingPhase
	{
		Set,
		Pitch,
		Release,
	};
};
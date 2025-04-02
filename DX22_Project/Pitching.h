// ==============================
//    インクルード部
// ==============================
#pragma once
#include "Defines.h"
#include "StrikeZone.h"
#include "PitchingCursor.h"
#include "Texture.h"

class CPitching
{
public:
	CPitching();
	~CPitching();
	void Update();
	void Draw();

public:
	// 投球の流れ
	enum class PitchingPhase
	{
		Set,	// セットポジション
		Pitch,	// ピッチング中
		Release,// リリース中
	};
	
public:
	// ==============================
	//    アクセサ
	// ==============================
	// 
	// ------------Setter------------
	/// <summary> SetCursor:CPitchingCursorクラスのインスタンスをセットする </summary>
	/// <param name="cursor:"> CPitchingCursorクラスのインスタンス </param>
	void SetCursor(CPitchingCursor* cursor);
	/// <summary> SetStrikeZone:CStrikeZoneクラスのインスタンスをセットする </summary>
	/// <param name="zone:"> CStrikeZoneクラスのインスタンス </param>
	void SetStrikeZone(CStrikeZone* zone);

	// ------------Getter------------
	/// <summary> GetPitchingPhase:現在の投球フェーズを取得する </summary>
	/// <returns> 現在の投球フェーズ </returns>
	PitchingPhase GetPitchingPhase();
	/// <summary> GetChatchTime:捕球までの時間を取得する </summary>
	/// <returns> 捕球までの時間 </returns>
	float GetChatchTime();

private:
	// テクスチャの種類
	enum class TexKind
	{
		ReleasePoint,	// リリースサークル
		PitchingCircle,	// ピッチングサークル

		Max
	};

	// コンポジション
	std::unique_ptr<Texture> m_pTexture[(int)TexKind::Max];
	std::unique_ptr<CStrikeZone> m_pStrikeZone;
	std::unique_ptr<CPitchingCursor> m_pPitchingCursor;

	// パラメータ
	SpriteParam m_tParam[(int)TexKind::Max];

	// メンバ変数
	int m_nPitchingPhase;	// 投球の流れ
	float m_fSpeed;			// 球速
	float m_fChatchTime;	// 捕球までの時間

private:
	// 内部処理
	// リリースポイント、ピッチングサークルの描画
	void DrawCircle();

};
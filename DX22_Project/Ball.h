// ==============================
//    インクルード部
// ==============================
#pragma once
#include <memory>
#include "GameObject.h"
#include "Camera.h"
#include "StrikeZone.h"
#include "Pitching.h"
#include "PitchingCursor.h"
#include "Texture.h"

// ==============================
//    前方宣言
// ==============================
class CBatting;

// ==============================
//    定数定義
// ==============================
constexpr DirectX::XMFLOAT3 ce_fBallPos = { 0.0f,-3.0f,30.0f };
constexpr DirectX::XMFLOAT3 ce_fBallEndPos = { 0.0f,-5.0f,148.0f };

// ==============================
//    シングルトンインスタンス
// ==============================
class CBall : public CGameObject
{
private:
	CBall();

public:
	virtual ~CBall();
	virtual void Update();
	virtual void Draw();
	virtual void SetModel(ModelParam param, Model* model, bool isAnime = false);

public:
	// ボールの状態
	enum class BallPhase
	{
		Batting,	// バッティング中
		InPlay,		// インプレー中
	};

public:
	// ==============================
	//    アクセサ
	// ==============================
	// 
	// ------------Setter------------
	/// <summary> SetPitching:CPitchingクラスのインスタンスをセットする </summary>
	/// <param name="pitching"> CPitchingクラスのインスタンス </param>
	void SetPitching(CPitching* pitching);
	/// <summary> SetPitchingCursor:CPitchingCursorクラスのインスタンスをセットする </summary>
	/// <param name="cursor"> CPitchingCursorクラスのインスタンス </param>
	void SetPitchingCursor(CPitchingCursor* cursor);
	/// <summary> SetBatting:CBattingクラスのインスタンスをセットする </summary>
	/// <param name="batting"> CBattingクラスのインスタンス </param>
	void SetBatting(CBatting* batting);
	/// <summary> SetPos:ボールの座標をセットする </summary>
	/// <param name="pos"> 座標 </param>
	void SetPos(DirectX::XMFLOAT3 pos);
	/// <summary> SetMove:ボールの進む方向と進行速度をセットする </summary>
	/// <param name="direction"> 進む方向 </param>
	void SetMove(DirectX::XMFLOAT3 direction);

	// ------------Getter------------
	/// <summary> GetCollision:ボールの球体当たり判定を取得する </summary>
	/// <returns> ボールの当たり判定情報(球体) </returns>
	Collision::Info GetCollision();
	/// <summary> GetLineCollision:ボールの軌跡当たり判定を取得する </summary>
	/// <returns> ボールの当たり判定情報(線) </returns>
	Collision::Info GetLineCollision();
	/// <summary> GetPos:ボールの座標を取得する </summary>
	/// <returns> ボールの座標 </returns>
	DirectX::XMFLOAT3 GetPos();
	/// <summary> GetIsFry:フライが上がっている状態か取得する</summary>
	/// <returns> true:フライ false:ゴロ </returns>
	bool GetIsFry();
	/// <summary> GetPhase:ボールの状態を取得する</summary>
	/// <returns> Batting:バッティング中 </returns>
	/// <returns> Inplay:インプレー中 </returns>
	BallPhase GetPhase();
	/// <summary> CBallクラスのシングルトンインスタンスを取得する</summary>
	/// <returns> CBallクラスのインスタンス </returns>
	static std::unique_ptr<CBall>& GetInstance();

public:
	/// <summary> OnCollision:ボールに当たった時の処理 </summary>
	/// <param name="collision"> 当たった結果 </param>
	void OnCollision(Collision::Result collision);

private:
	// コンポジション
	std::unique_ptr<Texture> m_pShadow;
	std::unique_ptr<Model> m_pModel;
	std::unique_ptr<CPitching> m_pPitching;
	std::unique_ptr<CPitchingCursor> m_pCursor;
	std::unique_ptr<CBatting> m_pBatting;

	// モデルのパラメータ
	ModelParam m_tParam;
	DirectX::XMFLOAT3 m_fMove;	// 移動量
	DirectX::XMFLOAT3 m_fShadowPos;	// 影の位置
	
	// メンバ変数
	int m_nPhase;	// ボールの状態
	bool m_bFry;	// フライ上がっている状態か

	// 当たり判定情報
	Collision::Info m_BallCollision;	// 球体当たり判定
	Collision::Info m_LucusCollision;	// 軌跡当たり判定

private:
	// 内部処理
	// バッティング時の処理
	void UpdateBatting();
	// インプレー時の処理
	void UpdateInPlay();
	// 影の描画処理
	void DrawShadow();
};

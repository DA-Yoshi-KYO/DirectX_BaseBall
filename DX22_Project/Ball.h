#pragma once
#include <memory>
#include "GameObject.h"
#include "Camera.h"
#include "StrikeZone.h"
#include "Pitching.h"
#include "PitchingCursor.h"

class CBatting;

constexpr DirectX::XMFLOAT3 ce_fBallPos = { 0.0f,-3.0f,65.0f };
constexpr DirectX::XMFLOAT3 ce_fBallEndPos = { 0.0f,-5.0f,148.0f };

class CBall : public CGameObject
{
public:
	CBall();
	~CBall();
	void Update() override;

public:
	// ボールの状態
	enum class BallPhase
	{
		Batting,	// バッティング中
		InPlay,		// インプレー中
	};

public:
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
	/// <summary> GetIsFry:フライが上がっている状態か取得する</summary>
	/// <returns> true:フライ false:ゴロ </returns>
	bool GetIsFry();
	/// <summary> GetPhase:ボールの状態を取得する</summary>
	/// <returns> Batting:バッティング中 </returns>
	/// <returns> Inplay:インプレー中 </returns>
	BallPhase GetPhase();

public:
	/// <summary> OnCollision:ボールに当たった時の処理 </summary>
	/// <param name="collision"> 当たった結果 </param>
	void OnCollision(Collision::Result collision);
	/// <summary> OnCollision:ボールに当たった時の処理 </summary>
	/// <param name="collision"> 当たった結果 </param>
	void OnFoulZone(Collision::Result collision);

private:

	// モデルのパラメータ
	DirectX::XMFLOAT3 m_fMove;	// 移動量
	DirectX::XMFLOAT2 m_fPitchPos;	// 投球場所
	DirectX::XMFLOAT2 m_fPredValue;	// 変化量
	
	// メンバ変数
	int m_nPhase;	// ボールの状態
	DirectX::XMFLOAT3 m_fFaulZoneBallPos;	// ファールゾーンに入った時のボールの座標
	bool m_bFry;	// フライ上がっている状態か
	bool m_bBallFaulZone;	// ファール判定

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

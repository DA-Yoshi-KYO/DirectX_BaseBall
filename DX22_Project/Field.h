// ==============================
//    インクルード部
// ==============================
#pragma once
#include "GameObject.h"
#include "Defines.h"
#include "Ball.h"

// ==============================
//    定数定義
// ==============================
constexpr float ce_fGroundY = -5.0f;	// グラウンドの見た目上の高さ
constexpr float ce_fFenceHeight = ce_fGroundY + 15.0f;	// フェンスの高さ
constexpr float ce_fInOutBorderZ = WORLD_AJUST;	// 内外野の境目

class CField : public CGameObject
{
public:
	CField();
	~CField();
	void Update() override;
	// ベースの種類
	enum class BaseKind
	{
		First,	// ファーストベース
		Second,	// セカンドベース
		Third,	// サードベース
		Home,	// ホームベース

		Max
	};

public:
	/// <summary> OnCollision:グラウンドに当たった時の処理 </summary>
	/// <param name="collision"> 当たった結果 </param>
	void OnCollision(Collision::Result collision);

private:
	// 当たり判定情報
	Collision::Info m_Ground;
	std::vector<Collision::Info> m_FirstBaseLine;	// 一塁線
	std::vector<Collision::Info> m_ThirdBaseLine;	// 三塁線
	std::vector<Collision::Info> m_LeftFoul;		// レフト側ファールゾーン
	std::vector<Collision::Info> m_RightFoul;		// ライト側ファールゾーン
	std::vector<Collision::Info> m_HomeRunZone;		// ホームランゾーン

private:
	// 内部処理
	// モデルの読み込み
	void InitModel();
	// 当たり判定情報の初期化
	void InitCollision();

};
